

#include "gtest/gtest.h"
import base_test;

import compilation_config;
import stl;
import logger;
import third_party;
import global_resource_usage;
import storage;
import hybridsearch_context;
import txn_manager;
import txn;
import extra_ddl_info;
import table_def;
import column_def;
import data_type;
import logical_type;
import column_vector;
import data_block;
import index_hnsw;
import index_secondary;
import statement_common;
import embedding_info;
import knn_expr;
import catalog;
import hybridsearch_exception;
import bg_task;
import txn_store;
import wal_manager;
import buffer_manager;
import internal_types;
import background_process;
import txn_state;

using namespace hybridsearch;

class OptimizeKnnTest : public BaseTestParamStr {
protected:
    void WaitCleanup(Storage *storage) {
        Catalog *catalog = storage->catalog();
        BufferManager *buffer_mgr = storage->buffer_manager();
        TxnManager *txn_mgr = storage->txn_manager();
        TxnTimeStamp visible_ts = txn_mgr->GetCleanupScanTS();
        auto cleanup_task = MakeShared<CleanupTask>(catalog, visible_ts, buffer_mgr);
        cleanup_task->Execute();
    }

    void WaitFlushFullCkp(Storage *storage) {
        WalManager *wal_mgr = storage->wal_manager();
        wal_mgr->Checkpoint(true /*is_full_checkpoint*/);
    }
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         OptimizeKnnTest,
                         ::testing::Values((std::string(test_data_path()) + "/config/test_optimize.toml").c_str(),
                                           (std::string(test_data_path()) + "/config/test_optimize_vfs_off.toml").c_str()));

TEST_P(OptimizeKnnTest, test1) {
    Storage *storage = hybridsearchContext::instance().storage();
    TxnManager *txn_mgr = storage->txn_manager();

    auto db_name = std::make_shared<std::string>("default_db");

    auto column_def1 = std::make_shared<ColumnDef>(0, std::make_shared<DataType>(LogicalType::kInteger), "col1", std::set<ConstraintType>());
    auto column_def2 =
        std::make_shared<ColumnDef>(1,
                                    std::make_shared<DataType>(LogicalType::kEmbedding, EmbeddingInfo::Make(EmbeddingDataType::kElemFloat, 4)),
                                    "col2",
                                    std::set<ConstraintType>());

    auto table_name = std::make_shared<std::string>("tb1");
    auto table_def = TableDef::Make(db_name, table_name, MakeShared<String>(), {column_def1, column_def2});

    auto index_name = std::make_shared<std::string>("idx1");

    {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
        txn->CreateTable(*db_name, table_def, ConflictType::kError);

        txn_mgr->CommitTxn(txn);
    }

    {
        Vector<String> column_names{"col2"};
        const String &file_name = "idx_file.idx";
        Vector<UniquePtr<InitParameter>> index_param_list; //
        Vector<InitParameter *> index_param_list_ptr;
        index_param_list.push_back(std::make_unique<InitParameter>(InitParameter{"metric", "l2"}));
        index_param_list.push_back(std::make_unique<InitParameter>(InitParameter{"encode", "plain"}));
        for (auto &param : index_param_list) {
            index_param_list_ptr.push_back(param.get());
        }
        // index_param_list

        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());

        auto index_hnsw = IndexHnsw::Make(index_name, MakeShared<String>("test comment"), file_name, column_names, index_param_list_ptr);
        auto [table_index_entry, status2] = txn->CreateIndexDef(table_entry, index_hnsw, ConflictType::kError);
        ASSERT_TRUE(status2.ok());

        txn_mgr->CommitTxn(txn);
    }

    auto DoAppend = [&]() {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);
        Vector<SharedPtr<ColumnVector>> column_vectors;
        for (SizeT i = 0; i < table_def->columns().size(); ++i) {
            SharedPtr<DataType> data_type = table_def->columns()[i]->type();
            column_vectors.push_back(MakeShared<ColumnVector>(data_type));
            column_vectors.back()->Initialize();
        }
        Vector<int> col1{2, 4, 6, 8};
        Vector<Vector<float>> col2{{0.1, 0.2, 0.3, -0.2}, {0.2, 0.1, 0.3, 0.4}, {0.3, 0.2, 0.1, 0.4}, {0.4, 0.3, 0.2, 0.1}};
        SizeT row_cnt = 4;
        for (SizeT i = 0; i < row_cnt; ++i) {
            column_vectors[0]->AppendByPtr(reinterpret_cast<const char *>(&col1[i]));
            column_vectors[1]->AppendByPtr(reinterpret_cast<const char *>(col2[i].data()));
        }
        auto data_block = DataBlock::Make();
        data_block->Init(column_vectors);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());

        status = txn->Append(*db_name, *table_name, data_block);
        ASSERT_TRUE(status.ok());
        txn_mgr->CommitTxn(txn);
    };

    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 2; ++i) {
            DoAppend();
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);

            auto [table_entry, status1] = txn->GetTableByName(*db_name, *table_name);
            ASSERT_TRUE(status1.ok());

            auto [table_index_entry, status] = txn->GetIndexByName(*db_name, *table_name, *index_name);
            ASSERT_TRUE(status.ok());

            TxnTableStore *txn_table_store = txn->GetTxnTableStore(table_entry);
            table_index_entry->MemIndexDump(txn, txn_table_store, true /*spill*/);

            txn_mgr->CommitTxn(txn);
        }
    }
    {
        Txn *txn = txn_mgr->BeginTxn(MakeUnique<String>("optimize index"), TransactionType::kNormal);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());
        table_entry->OptimizeIndex(txn);

        txn_mgr->CommitTxn(txn);
    }

    WaitFlushFullCkp(storage);
    WaitCleanup(storage);
    {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kNormal);

        auto [table_entry, status1] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status1.ok());

        auto [table_index_entry, status] = txn->GetIndexByName(*db_name, *table_name, *index_name);
        ASSERT_TRUE(status.ok());

        auto &segment_index_entries = table_index_entry->index_by_segment();
        ASSERT_EQ(segment_index_entries.size(), 1ul);
        auto &segment_index_entry = segment_index_entries.begin()->second;

        auto [chunk_index_entries, memory_index_entry] = segment_index_entry->GetHnswIndexSnapshot();
        ASSERT_EQ(chunk_index_entries.size(), 1ul);
        auto &chunk_index_entry = chunk_index_entries[0];
        ASSERT_EQ(chunk_index_entry->row_count_, 24u);

        ASSERT_EQ(memory_index_entry.get(), nullptr);

        txn_mgr->CommitTxn(txn);
    }
}

TEST_P(OptimizeKnnTest, test_secondary_index_optimize) {
    Storage *storage = hybridsearchContext::instance().storage();
    TxnManager *txn_mgr = storage->txn_manager();

    auto db_name = std::make_shared<std::string>("default_db");
    auto column_def1 = std::make_shared<ColumnDef>(0, std::make_shared<DataType>(LogicalType::kInteger), "col1", std::set<ConstraintType>());
    auto table_name = std::make_shared<std::string>("tb1");
    auto table_def = TableDef::Make(db_name, table_name, MakeShared<String>(), {column_def1});
    auto index_name = std::make_shared<std::string>("idx1");

    {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
        txn->CreateTable(*db_name, table_def, ConflictType::kError);
        txn_mgr->CommitTxn(txn);
    }

    {
        Vector<String> column_names{"col1"};
        const String &file_name = "idx_file.idx";
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);
        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());
        auto index_secondary = IndexSecondary::Make(index_name, MakeShared<String>("test comment"), file_name, column_names);
        auto [table_index_entry, status2] = txn->CreateIndexDef(table_entry, index_secondary, ConflictType::kError);
        ASSERT_TRUE(status2.ok());
        txn_mgr->CommitTxn(txn);
    }

    auto DoAppend = [&]() {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);
        Vector<SharedPtr<ColumnVector>> column_vectors;
        for (SizeT i = 0; i < table_def->columns().size(); ++i) {
            SharedPtr<DataType> data_type = table_def->columns()[i]->type();
            column_vectors.push_back(MakeShared<ColumnVector>(data_type));
            column_vectors.back()->Initialize();
        }
        Vector<int> col1{2, 4, 6, 8};
        SizeT row_cnt = 4;
        for (SizeT i = 0; i < row_cnt; ++i) {
            column_vectors[0]->AppendByPtr(reinterpret_cast<const char *>(&col1[i]));
        }
        auto data_block = DataBlock::Make();
        data_block->Init(column_vectors);
        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());
        status = txn->Append(*db_name, *table_name, data_block);
        ASSERT_TRUE(status.ok());
        txn_mgr->CommitTxn(txn);
    };

    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 2; ++i) {
            DoAppend();
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);
            auto [table_entry, status1] = txn->GetTableByName(*db_name, *table_name);
            ASSERT_TRUE(status1.ok());
            auto [table_index_entry, status] = txn->GetIndexByName(*db_name, *table_name, *index_name);
            ASSERT_TRUE(status.ok());
            TxnTableStore *txn_table_store = txn->GetTxnTableStore(table_entry);
            table_index_entry->MemIndexDump(txn, txn_table_store, true /*spill*/);
            txn_mgr->CommitTxn(txn);
        }
    }
    {
        Txn *txn = txn_mgr->BeginTxn(MakeUnique<String>("optimize index"), TransactionType::kNormal);
        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status.ok());
        table_entry->OptimizeIndex(txn);
        txn_mgr->CommitTxn(txn);
    }
    WaitFlushFullCkp(storage);
    WaitCleanup(storage);
    {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kNormal);
        auto [table_entry, status1] = txn->GetTableByName(*db_name, *table_name);
        ASSERT_TRUE(status1.ok());
        auto [table_index_entry, status] = txn->GetIndexByName(*db_name, *table_name, *index_name);
        ASSERT_TRUE(status.ok());
        auto &segment_index_entries = table_index_entry->index_by_segment();
        ASSERT_EQ(segment_index_entries.size(), 1ul);
        auto &segment_index_entry = segment_index_entries.begin()->second;
        auto [chunk_index_entries, memory_index_entry] = segment_index_entry->GetSecondaryIndexSnapshot();
        ASSERT_EQ(chunk_index_entries.size(), 1ul);
        auto &chunk_index_entry = chunk_index_entries[0];
        ASSERT_EQ(chunk_index_entry->row_count_, 24u);
        ASSERT_EQ(memory_index_entry.get(), nullptr);
        txn_mgr->CommitTxn(txn);
    }
}
