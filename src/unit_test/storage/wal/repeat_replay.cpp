

#include "gtest/gtest.h"

import base_test;

import stl;
import global_resource_usage;
import storage;
import hybridsearch_context;
import compilation_config;
import txn_manager;
import extra_ddl_info;
import hybridsearch_exception;
import log_file;
import config;
import bg_task;
import background_process;
import default_values;
import status;
import logger;
import column_def;
import column_vector;
import data_type;
import logical_type;
import table_def;
import data_block;
import block_entry;
import txn;
import catalog;
import segment_entry;
import value;
import internal_types;
import buffer_manager;
import physical_import;
import txn_state;

using namespace hybridsearch;

class RepeatReplayTest : public BaseTestParamStr {
protected:
    static std::shared_ptr<std::string> close_ckp_config() {
        return GetParam() == BaseTestParamStr::NULL_CONFIG_PATH
                   ? std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp.toml")
                   : std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp_vfs_off.toml");
    }

    void SetUp() override { CleanupDbDirs(); }

    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         RepeatReplayTest,
                         ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH, BaseTestParamStr::VFS_OFF_CONFIG_PATH));

TEST_P(RepeatReplayTest, append) {
    std::shared_ptr<std::string> config_path = RepeatReplayTest::close_ckp_config();

    auto db_name = std::make_shared<std::string>("default_db");
    auto column_def1 = std::make_shared<ColumnDef>(0, std::make_shared<DataType>(LogicalType::kInteger), "col1", std::set<ConstraintType>());
    auto column_def2 = std::make_shared<ColumnDef>(1, std::make_shared<DataType>(LogicalType::kVarchar), "col2", std::set<ConstraintType>());
    auto table_name = std::make_shared<std::string>("tb1");
    auto table_def = TableDef::Make(db_name, table_name, MakeShared<String>(), {column_def1, column_def2});

    auto TestAppend = [&](TxnManager *txn_mgr) {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);

        Vector<SharedPtr<ColumnVector>> column_vectors;
        for (SizeT i = 0; i < table_def->columns().size(); ++i) {
            SharedPtr<DataType> data_type = table_def->columns()[i]->type();
            column_vectors.push_back(MakeShared<ColumnVector>(data_type));
            column_vectors.back()->Initialize();
        }
        {
            int v1 = 1;
            column_vectors[0]->AppendByPtr(reinterpret_cast<const_ptr_t>(&v1));
        }
        {
            std::string v2 = "v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2";
            column_vectors[1]->AppendByStringView(v2);
        }
        auto data_block = DataBlock::Make();
        data_block->Init(column_vectors);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        EXPECT_TRUE(status.ok());

        status = txn->Append(*db_name, *table_name, data_block);
        ASSERT_TRUE(status.ok());
        txn_mgr->CommitTxn(txn);
    };
    auto CheckTable = [&](TxnManager *txn_mgr, size_t row_cnt) {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get table"), TransactionType::kRead);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        EXPECT_TRUE(status.ok());

        EXPECT_EQ(table_entry->row_count(), row_cnt);
        ASSERT_EQ(table_entry->segment_map().size(), 1ul);
        {
            auto &segment_entry = table_entry->segment_map().begin()->second;
            ASSERT_EQ(segment_entry->row_count(), row_cnt);
            ASSERT_EQ(segment_entry->block_entries().size(), 1ul);
            {
                BlockEntry *block_entry = segment_entry->block_entries()[0].get();
                EXPECT_EQ(block_entry->row_count(), row_cnt);
                ASSERT_EQ(block_entry->columns().size(), 2ul);
                {
                    auto &col2 = block_entry->columns()[1];
                    EXPECT_EQ(col2->OutlineBufferCount(), 1ul);
                }
            }
        }

        txn_mgr->CommitTxn(txn);
    };

    {
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            txn->CreateTable(*db_name, table_def, ConflictType::kError);

            auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        TestAppend(txn_mgr);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    { // replay with no checkpoint, only replay wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        CheckTable(txn_mgr, 1);
        TestAppend(txn_mgr);
        CheckTable(txn_mgr, 2);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    { // replay with full checkpoint + wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        CheckTable(txn_mgr, 2);
        { //  manually add delta checkpoint
            auto *txn_force_ckp = txn_mgr->BeginTxn(MakeUnique<String>("full checkpoint"), TransactionType::kCheckpoint);
            auto force_ckp_task = MakeShared<ForceCheckpointTask>(txn_force_ckp, false /*is_full_checkpoint*/);
            storage->bg_processor()->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn_force_ckp);
        }
        TestAppend(txn_mgr);
        CheckTable(txn_mgr, 3);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    for (int i = 0; i < 2; ++i) { // replay with full checkpoint + delta checkpoint + wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        CheckTable(txn_mgr, 3);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
}

TEST_P(RepeatReplayTest, import) {
    std::shared_ptr<std::string> config_path = RepeatReplayTest::close_ckp_config();

    auto db_name = std::make_shared<std::string>("default_db");
    auto column_def1 = std::make_shared<ColumnDef>(0, std::make_shared<DataType>(LogicalType::kInteger), "col1", std::set<ConstraintType>());
    auto column_def2 = std::make_shared<ColumnDef>(1, std::make_shared<DataType>(LogicalType::kVarchar), "col2", std::set<ConstraintType>());
    auto table_name = std::make_shared<std::string>("tb1");
    auto table_def = TableDef::Make(db_name, table_name, MakeShared<String>(), {column_def1, column_def2});

    auto TestImport = [&](TxnManager *txn_mgr, BufferManager *buffer_mgr) {
        Txn *txn = txn_mgr->BeginTxn(MakeUnique<String>("import table"), TransactionType::kNormal);
        auto [table_info, status] = txn->GetTableInfo(*db_name, *table_name);
        ASSERT_TRUE(status.ok());
        auto [segment_entry, segment_status] = txn->MakeNewSegment(*db_name, *table_name);
        auto block_entry = BlockEntry::NewBlockEntry(segment_entry.get(), 0 /*block_id*/, 0 /*checkpoint_ts*/, 2 /*column_count*/, txn);

        Vector<SharedPtr<ColumnVector>> columns_vector;
        {
            SharedPtr<ColumnVector> column_vector = ColumnVector::Make(MakeShared<DataType>(LogicalType::kInteger));
            column_vector->Initialize();
            Value v = Value::MakeInt(static_cast<IntegerT>(1));
            column_vector->AppendValue(v);
            columns_vector.push_back(column_vector);
        }
        {
            SharedPtr<ColumnVector> column_vector = ColumnVector::Make(MakeShared<DataType>(LogicalType::kVarchar));
            column_vector->Initialize();
            Value v = Value::MakeVarchar("v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2v2");
            column_vector->AppendValue(v);
            columns_vector.push_back(column_vector);
        }
        for (int i = 0; i < 2; ++i) {
            ColumnVector col = block_entry->GetColumnVector(buffer_mgr, i);
            col.AppendWith(*columns_vector[i], 0 /*offset*/, 1 /*rows*/);
        }
        block_entry->IncreaseRowCount(1 /*rows*/);
        segment_entry->AppendBlockEntry(std::move(block_entry));

        PhysicalImport::SaveSegmentData(table_info.get(), txn, segment_entry);
        txn_mgr->CommitTxn(txn);
    };
    auto CheckTable = [&](TxnManager *txn_mgr, size_t row_cnt) {
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);

        auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
        EXPECT_TRUE(status.ok());

        EXPECT_EQ(table_entry->row_count(), row_cnt);
        ASSERT_EQ(table_entry->segment_map().size(), row_cnt);
        {
            auto &segment_entry = table_entry->segment_map().begin()->second;
            ASSERT_EQ(segment_entry->row_count(), 1ul);
            ASSERT_EQ(segment_entry->block_entries().size(), 1ul);
            {
                BlockEntry *block_entry = segment_entry->block_entries()[0].get();
                EXPECT_EQ(block_entry->row_count(), 1ul);
            }
        }

        txn_mgr->CommitTxn(txn);
    };

    {
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        BufferManager *buffer_mgr = storage->buffer_manager();
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            txn->CreateTable(*db_name, table_def, ConflictType::kError);

            auto [table_entry, status] = txn->GetTableByName(*db_name, *table_name);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        TestImport(txn_mgr, buffer_mgr);
        CheckTable(txn_mgr, 1);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    { // replay with no checkpoint, only replay wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        BufferManager *buffer_mgr = storage->buffer_manager();
        CheckTable(txn_mgr, 1);
        TestImport(txn_mgr, buffer_mgr);
        CheckTable(txn_mgr, 2);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    { // replay with full checkpoint + wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        BufferManager *buffer_mgr = storage->buffer_manager();
        CheckTable(txn_mgr, 2);
        { //  manually add delta checkpoint
            auto *txn_force_ckp = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            auto force_ckp_task = MakeShared<ForceCheckpointTask>(txn_force_ckp, false /*is_full_checkpoint*/);
            storage->bg_processor()->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn_force_ckp);
        }
        TestImport(txn_mgr, buffer_mgr);
        CheckTable(txn_mgr, 3);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
    for (int i = 0; i < 2; ++i) { // replay with full checkpoint + delta checkpoint + wal
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2(); // auto full checkpoint when initialize
        Storage *storage = hybridsearchContext::instance().storage();

        TxnManager *txn_mgr = storage->txn_manager();
        CheckTable(txn_mgr, 3);
        hybridsearch::hybridsearchContext::instance().UnInit();
    }
}