

#include "type/complex/embedding_type.h"
#include "gtest/gtest.h"
import base_test;

import stl;
import global_resource_usage;
import storage;
import hybridsearch_context;

import txn_manager;
import table_def;
import data_block;
import value;
import txn_store;
import buffer_manager;
import meta_state;
import wal_entry;
import hybridsearch_exception;
import status;
import column_vector;
import physical_import;
import txn;
import catalog;
import index_base;
import index_ivf;
import index_hnsw;
import index_full_text;
import bg_task;
import background_process;
import default_values;
import base_table_ref;
import internal_types;
import logical_type;
import embedding_info;
import extra_ddl_info;
import knn_expr;
import column_def;
import statement_common;
import data_type;

import segment_entry;
import block_entry;
import block_column_entry;
import table_index_entry;
import base_entry;
import compilation_config;
import compaction_process;
import txn_state;

using namespace hybridsearch;

class WalReplayTest : public BaseTestParamStr {
protected:
    static std::shared_ptr<std::string> config_path() {
        return GetParam() == BaseTestParamStr::NULL_CONFIG_PATH
                   ? std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp.toml")
                   : std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp_vfs_off.toml");
    }

    void SetUp() override {
        CleanupDbDirs();
        tree_cmd = "tree ";
        tree_cmd += GetFullDataDir();
    }

    void TearDown() override {
        // system(tree_cmd.c_str());
        //        RemoveDbDirs();
    }

    String tree_cmd;
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         WalReplayTest,
                         ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH, BaseTestParamStr::VFS_OFF_CONFIG_PATH));

TEST_P(WalReplayTest, wal_replay_database) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            txn->CreateDatabase(MakeShared<String>("db1"), ConflictType::kIgnore, MakeShared<String>());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            txn->CreateDatabase(MakeShared<String>("db2"), ConflictType::kIgnore, MakeShared<String>());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            txn->CreateDatabase(MakeShared<String>("db3"), ConflictType::kIgnore, MakeShared<String>());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            txn->CreateDatabase(MakeShared<String>("db4"), ConflictType::kIgnore, MakeShared<String>());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            txn->CreateDatabase(MakeShared<String>("db5"), ConflictType::kIgnore, MakeShared<String>());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop db"), TransactionType::kNormal);
            txn->DropDatabase("db1", ConflictType::kIgnore);
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }

    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop db"), TransactionType::kNormal);
            Status status = txn->DropDatabase("db4", ConflictType::kError);
            EXPECT_EQ(status.ok(), true);
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
            Status status = txn->CreateDatabase(MakeShared<String>("db1"), ConflictType::kError, MakeShared<String>());
            EXPECT_EQ(status.ok(), true);
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

TEST_P(WalReplayTest, wal_replay_tables) {

    Vector<SharedPtr<ColumnDef>> columns;
    {
        i64 column_id = 0;
        {
            std::set<ConstraintType> constraints;
            constraints.insert(ConstraintType::kUnique);
            constraints.insert(ConstraintType::kNotNull);
            auto column_def_ptr =
                MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
            columns.emplace_back(column_def_ptr);
        }
        {
            std::set<ConstraintType> constraints;
            constraints.insert(ConstraintType::kPrimaryKey);
            auto column_def_ptr =
                MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kBigInt)), "big_int_col", constraints);
            columns.emplace_back(column_def_ptr);
        }
        {
            std::set<ConstraintType> constraints;
            constraints.insert(ConstraintType::kNotNull);
            auto column_def_ptr = MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kDouble)), "double_col", constraints);
            columns.emplace_back(column_def_ptr);
        }
    }
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        {
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl1"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto tbl2_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl2"), MakeShared<String>(), columns);
            auto *txn2 = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status2 = txn2->CreateTable("default_db", std::move(tbl2_def), ConflictType::kIgnore);
            EXPECT_TRUE(status2.ok());
            txn_mgr->CommitTxn(txn2);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop table"), TransactionType::kNormal);
            Status status = txn->DropTableCollectionByName("default_db", "tbl2", ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto tbl3_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl3"), MakeShared<String>(), columns);
            auto *txn3 = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn3->CreateTable("default_db", std::move(tbl3_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn3);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        {
            auto tbl2_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl2"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl2_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop table"), TransactionType::kNormal);
            Status status = txn->DropTableCollectionByName("default_db", "tbl3", ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

TEST_P(WalReplayTest, wal_replay_append) {
    SizeT row_count = 2;
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        Vector<SharedPtr<ColumnDef>> columns;
        {
            i64 column_id = 0;
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kUnique);
                constraints.insert(ConstraintType::kNotNull);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kPrimaryKey);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kBigInt)), "big_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kNotNull);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kDouble)), "double_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
        }

        {
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl1"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto tbl3_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl3"), MakeShared<String>(), columns);
            auto *txn3 = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn3->CreateTable("default_db", std::move(tbl3_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn3);
        }
        {
            auto tbl4_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl4"), MakeShared<String>(), columns);
            auto *txn4 = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn4->CreateTable("default_db", std::move(tbl4_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn4);
        }
        {
            auto *txn5 = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);
            SharedPtr<DataBlock> input_block = MakeShared<DataBlock>();

            // Prepare the input data block
            Vector<SharedPtr<DataType>> column_types;
            column_types.emplace_back(MakeShared<DataType>(LogicalType::kTinyInt));
            column_types.emplace_back(MakeShared<DataType>(LogicalType::kBigInt));
            column_types.emplace_back(MakeShared<DataType>(LogicalType::kDouble));

            input_block->Init(column_types, row_count);
            for (SizeT i = 0; i < row_count; ++i) {
                input_block->AppendValue(0, Value::MakeTinyInt(static_cast<i8>(i)));
            }

            for (SizeT i = 0; i < row_count; ++i) {
                input_block->AppendValue(1, Value::MakeBigInt(static_cast<i64>(i)));
            }

            for (SizeT i = 0; i < row_count; ++i) {
                input_block->AppendValue(2, Value::MakeDouble(static_cast<f64>(i)));
            }
            input_block->Finalize();
            EXPECT_EQ(input_block->Finalized(), true);
            auto [table_entry, status] = txn5->GetTableByName("default_db", "tbl4");
            EXPECT_TRUE(status.ok());
            txn5->Append("default_db", "tbl4", input_block);
            txn_mgr->CommitTxn(txn5);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn);
        }
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    // Restart the db instance
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        Vector<SharedPtr<ColumnDef>> columns;
        {
            i64 column_id = 0;
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kUnique);
                constraints.insert(ConstraintType::kNotNull);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
        }
        {
            auto tbl5_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl5"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl5_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn->BeginTS();
            auto [table_entry, status] = txn->GetTableByName("default_db", "tbl4");
            EXPECT_NE(table_entry, nullptr);

            auto segment_entry = table_entry->GetSegmentByID(0, begin_ts);
            EXPECT_NE(segment_entry, nullptr);
            EXPECT_EQ(segment_entry->segment_id(), 0u);
            EXPECT_EQ(segment_entry->row_count(), row_count);

            auto *block_entry = segment_entry->GetBlockEntryByID(0).get();
            EXPECT_EQ(block_entry->block_id(), 0u);
            EXPECT_EQ(block_entry->row_count(), row_count);

            ColumnVector col0 = block_entry->GetConstColumnVector(storage->buffer_manager(), 0);
            ColumnVector col1 = block_entry->GetConstColumnVector(storage->buffer_manager(), 1);
            ColumnVector col2 = block_entry->GetConstColumnVector(storage->buffer_manager(), 2);

            for (SizeT i = 0; i < row_count; ++i) {
                Value v0 = col0.GetValue(i);
                EXPECT_EQ(v0.GetValue<TinyIntT>(), static_cast<i8>(i));

                Value v1 = col1.GetValue(i);
                EXPECT_EQ(v1.GetValue<BigIntT>(), static_cast<i64>(i));

                Value v2 = col2.GetValue(i);
                EXPECT_EQ(v2.GetValue<DoubleT>(), static_cast<f64>(i));
            }

            txn_mgr->CommitTxn(txn);
        }
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

TEST_P(WalReplayTest, wal_replay_import) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        BufferManager *buffer_manager = storage->buffer_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        Vector<SharedPtr<ColumnDef>> columns;
        {
            i64 column_id = 0;
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kUnique);
                constraints.insert(ConstraintType::kNotNull);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kPrimaryKey);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kBigInt)), "big_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kNotNull);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kDouble)), "double_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
        }
        int column_count = columns.size();

        {
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl1"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto tbl2_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl2"), MakeShared<String>(), columns);
            auto *txn2 = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn2->CreateTable("default_db", std::move(tbl2_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn2);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn);
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            auto tbl3_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl3"), MakeShared<String>(), columns);
            Status status = txn->CreateTable("default_db", std::move(tbl3_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        {
            auto txn4 = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);

            auto [table_info, status] = txn4->GetTableInfo("default_db", "tbl1");
            EXPECT_NE(table_info, nullptr);
            auto [segment_entry, segment_status] = txn4->MakeNewSegment("default_db", "tbl1");
            EXPECT_TRUE(segment_status.ok());
            EXPECT_EQ(segment_entry->segment_id(), 0u);
            auto block_entry = BlockEntry::NewBlockEntry(segment_entry.get(), 0, 0, column_count, txn4);
            // auto last_block_entry = segment_entry->GetLastEntry();

            Vector<SharedPtr<ColumnVector>> columns_vector;
            {
                SharedPtr<ColumnVector> column_vector = ColumnVector::Make(MakeShared<DataType>(LogicalType::kTinyInt));
                column_vector->Initialize();
                Value v = Value::MakeTinyInt(static_cast<TinyIntT>(1));
                column_vector->AppendValue(v);
                columns_vector.push_back(column_vector);
            }
            {
                SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kBigInt));
                column->Initialize();
                Value v = Value::MakeBigInt(static_cast<BigIntT>(22));
                column->AppendValue(v);
                columns_vector.push_back(column);
            }
            {
                SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kDouble));
                column->Initialize();
                Value v = Value::MakeDouble(static_cast<DoubleT>(f64(3)) + 0.33f);
                column->AppendValue(v);
                columns_vector.push_back(column);
            }

            {
                auto column_type1 = block_entry->GetColumnBlockEntry(0)->column_type().get();
                EXPECT_EQ(column_type1->type(), LogicalType::kTinyInt);
                SizeT data_type_size = columns_vector[0]->data_type_size_;
                EXPECT_EQ(data_type_size, 1u);
                ColumnVector col = block_entry->GetColumnVector(buffer_manager, 0);
                col.AppendWith(*columns_vector[0], 0, 1);
            }
            {
                auto column_type2 = block_entry->GetColumnBlockEntry(1)->column_type().get();
                EXPECT_EQ(column_type2->type(), LogicalType::kBigInt);
                SizeT data_type_size = columns_vector[1]->data_type_size_;
                EXPECT_EQ(data_type_size, 8u);
                ColumnVector col = block_entry->GetColumnVector(buffer_manager, 1);
                col.AppendWith(*columns_vector[1], 0, 1);
            }
            {
                auto column_type3 = block_entry->GetColumnBlockEntry(2)->column_type().get();
                EXPECT_EQ(column_type3->type(), LogicalType::kDouble);
                SizeT data_type_size = columns_vector[2]->data_type_size_;
                EXPECT_EQ(data_type_size, 8u);
                ColumnVector col = block_entry->GetColumnVector(buffer_manager, 2);
                col.AppendWith(*columns_vector[2], 0, 1);
            }

            block_entry->IncreaseRowCount(1);
            segment_entry->AppendBlockEntry(std::move(block_entry));

            PhysicalImport::SaveSegmentData(table_info.get(), txn4, segment_entry);
            txn_mgr->CommitTxn(txn4);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    // Restart the db instance
    //    system(tree_cmd.c_str());
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        BufferManager *buffer_manager = storage->buffer_manager();

        {
            auto txn = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn->BeginTS();

            Vector<ColumnID> column_ids{0, 1, 2};
            auto [table_entry, status] = txn->GetTableByName("default_db", "tbl1");
            EXPECT_NE(table_entry, nullptr);
            auto segment_entry = table_entry->GetSegmentByID(0, begin_ts);
            EXPECT_NE(segment_entry, nullptr);
            EXPECT_EQ(segment_entry->segment_id(), 0u);
            auto *block_entry = segment_entry->GetBlockEntryByID(0).get();
            EXPECT_EQ(block_entry->block_id(), 0u);
            EXPECT_EQ(block_entry->row_count(), 1u);

            BlockColumnEntry *column2 = block_entry->GetColumnBlockEntry(2);

            ColumnVector col0 = block_entry->GetConstColumnVector(buffer_manager, 0);
            Value v0 = col0.GetValue(0);
            EXPECT_EQ(v0.GetValue<TinyIntT>(), 1);

            ColumnVector col1 = block_entry->GetConstColumnVector(buffer_manager, 1);
            Value v1 = col1.GetValue(0);
            EXPECT_EQ(v1.GetValue<BigIntT>(), (i64)(22));

            ColumnVector col2 = block_entry->GetConstColumnVector(buffer_manager, 2);
            Value v2 = col2.GetValue(0);
            DataType *col2_type = column2->column_type().get();
            EXPECT_EQ(col2_type->type(), LogicalType::kDouble);
            EXPECT_EQ(v2.GetValue<DoubleT>(), (f64)(3) + 0.33f);

            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

// FIXME: The test case diverges from the original intent.
// TODO: Support compact for vfs
TEST_F(WalReplayTest, wal_replay_compact) {
    std::shared_ptr<std::string> config_path = std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp.toml");
    u64 test_segment_n = 2;
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        BufferManager *buffer_manager = storage->buffer_manager();
        TxnManager *txn_mgr = storage->txn_manager();
        CompactionProcessor *compaction_processor = storage->compaction_processor();

        Vector<SharedPtr<ColumnDef>> columns;
        {
            i64 column_id = 0;
            {
                std::set<ConstraintType> constraints;
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
                columns.emplace_back(column_def_ptr);
            }
        }
        int column_count = 1;
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("tbl1"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }

        for (u64 i = 0; i < test_segment_n; ++i) { // add 2 segments
            auto txn2 = txn_mgr->BeginTxn(MakeUnique<String>("insert table"), TransactionType::kNormal);

            auto [table_info, status] = txn2->GetTableInfo("default_db", "tbl1");
            EXPECT_NE(table_info, nullptr);

            auto [segment_entry, segment_status] = txn2->MakeNewSegment("default_db", "tbl1");
            EXPECT_EQ(segment_entry->segment_id(), i);

            auto block_entry = BlockEntry::NewBlockEntry(segment_entry.get(), 0, 0, column_count, txn2);

            Vector<SharedPtr<ColumnVector>> column_vectors;
            {
                SharedPtr<ColumnVector> column_vector = ColumnVector::Make(MakeShared<DataType>(LogicalType::kTinyInt));
                column_vector->Initialize();
                Value v = Value::MakeTinyInt(static_cast<TinyIntT>(1));
                column_vector->AppendValue(v);
                column_vectors.push_back(column_vector);
            }

            {
                auto column_type0 = block_entry->GetColumnBlockEntry(0)->column_type().get();
                EXPECT_EQ(column_type0->type(), LogicalType::kTinyInt);
                SizeT data_type_size = column_vectors[0]->data_type_size_;
                EXPECT_EQ(data_type_size, 1u);
                ColumnVector col = block_entry->GetColumnVector(buffer_manager, 0);
                col.AppendWith(*column_vectors[0], 0, 1);
                block_entry->IncreaseRowCount(1);
            }
            segment_entry->AppendBlockEntry(std::move(block_entry));

            PhysicalImport::SaveSegmentData(table_info.get(), txn2, segment_entry);
            txn_mgr->CommitTxn(txn2);
        }

        { // add compact
            auto commit_ts = compaction_processor->ManualDoCompact("default_db", "tbl1", false);
            EXPECT_NE(commit_ts, 0u);
        }
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    // Restart db instance
    //    system(tree_cmd.c_str());
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        {
            auto txn = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn->BeginTS();

            auto [table_entry, status] = txn->GetTableByName("default_db", "tbl1");
            EXPECT_NE(table_entry, nullptr);

            for (u64 i = 0; i < test_segment_n; ++i) {
                auto segment = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_EQ(segment, nullptr);
            }
            auto compact_segment = table_entry->GetSegmentByID(test_segment_n, begin_ts);
            EXPECT_NE(compact_segment, nullptr);
            EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);
            EXPECT_EQ(compact_segment->row_count(), test_segment_n);

            auto block_entry = compact_segment->GetBlockEntryByID(0).get();
            EXPECT_NE(block_entry, nullptr);
            EXPECT_EQ(block_entry->row_count(), test_segment_n);
            txn_mgr->CommitTxn(txn);
        }
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

TEST_P(WalReplayTest, wal_replay_create_index_IvfFlat) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        // CREATE TABLE test_annivfflat (col1 embedding(float,128));
        {
            Vector<SharedPtr<ColumnDef>> columns;
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kNotNull);
                i64 column_id = 0;
                auto embeddingInfo = MakeShared<EmbeddingInfo>(EmbeddingDataType::kElemFloat, 128);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id, MakeShared<DataType>(LogicalType::kEmbedding, embeddingInfo), "col1", constraints);
                columns.emplace_back(column_def_ptr);
            }
            auto tbl1_def =
                MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("test_annivfflat"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kError);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        // CreateIndex
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);

            Vector<String> columns1{"col1"};
            Vector<InitParameter *> parameters1;
            parameters1.emplace_back(new InitParameter("metric", "l2"));
            parameters1.emplace_back(new InitParameter("plain_storage_data_type", "f32"));

            SharedPtr<String> index_name = MakeShared<String>("idx1");
            auto index_base_ivf = IndexIVF::Make(index_name, MakeShared<String>("test comment"), "idx1_tbl1", columns1, parameters1);
            for (auto *init_parameter : parameters1) {
                delete init_parameter;
            }

            const String &db_name = "default_db";
            const String &table_name = "test_annivfflat";
            ConflictType conflict_type = ConflictType::kError;
            bool prepare = false;
            auto [table_entry, table_status] = txn->GetTableByName(db_name, table_name);
            EXPECT_EQ(table_status.ok(), true);
            {
                auto table_ref = BaseTableRef::FakeTableRef(txn, db_name, table_name);
                auto result = txn->CreateIndexDef(table_entry, index_base_ivf, conflict_type);
                auto *table_index_entry = std::get<0>(result);
                auto status = std::get<1>(result);
                EXPECT_EQ(status.ok(), true);
                txn->CreateIndexPrepare(table_index_entry, table_ref.get(), prepare);
                txn->CreateIndexFinish(table_entry, table_index_entry);
            }
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    ////////////////////////////////
    /// Restart the db instance...
    ////////////////////////////////
    //    system(tree_cmd.c_str());
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        {
            auto txn = txn_mgr->BeginTxn(MakeUnique<String>("get index"), TransactionType::kRead);
            Vector<ColumnID> column_ids{0};
            auto [table_entry, status1] = txn->GetTableByName("default_db", "test_annivfflat");
            EXPECT_TRUE(status1.ok());
            auto [index_entry, status2] = table_entry->GetIndex("idx1", txn->TxnID(), txn->BeginTS());
            ASSERT_TRUE(status2.ok());
            EXPECT_EQ(*index_entry->index_base()->index_name_, "idx1");
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}

TEST_P(WalReplayTest, wal_replay_create_index_hnsw) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        // BufferManager *buffer_manager = storage->buffer_manager();

        // CREATE TABLE test_hnsw (col1 embedding(float,128));
        {
            Vector<SharedPtr<ColumnDef>> columns;
            {
                std::set<ConstraintType> constraints;
                constraints.insert(ConstraintType::kNotNull);
                i64 column_id = 0;
                auto embeddingInfo = MakeShared<EmbeddingInfo>(EmbeddingDataType::kElemFloat, 128);
                auto column_def_ptr =
                    MakeShared<ColumnDef>(column_id, MakeShared<DataType>(LogicalType::kEmbedding, embeddingInfo), "col1", constraints);
                columns.emplace_back(column_def_ptr);
            }
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>("test_hnsw"), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);
            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kError);
            EXPECT_TRUE(status.ok());
            txn_mgr->CommitTxn(txn);
        }
        // CreateIndex
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);

            Vector<String> columns1{"col1"};
            Vector<InitParameter *> parameters1;
            parameters1.emplace_back(new InitParameter("metric", "l2"));
            parameters1.emplace_back(new InitParameter("encode", "plain"));
            parameters1.emplace_back(new InitParameter("m", "16"));
            parameters1.emplace_back(new InitParameter("ef_construction", "200"));

            SharedPtr<String> index_name = MakeShared<String>("hnsw_index");
            auto index_base_hnsw = IndexHnsw::Make(index_name, MakeShared<String>("test comment"), "hnsw_index_test_hnsw", columns1, parameters1);
            for (auto *init_parameter : parameters1) {
                delete init_parameter;
            }

            const String &db_name = "default_db";
            const String &table_name = "test_hnsw";
            ConflictType conflict_type = ConflictType::kError;
            bool prepare = false;
            auto [table_entry, table_status] = txn->GetTableByName(db_name, table_name);
            EXPECT_EQ(table_status.ok(), true);
            {
                auto table_ref = BaseTableRef::FakeTableRef(txn, db_name, table_name);
                auto result = txn->CreateIndexDef(table_entry, index_base_hnsw, conflict_type);
                auto *table_index_entry = std::get<0>(result);
                auto status = std::get<1>(result);
                EXPECT_EQ(status.ok(), true);
                txn->CreateIndexPrepare(table_index_entry, table_ref.get(), prepare);
                txn->CreateIndexFinish(table_entry, table_index_entry);
            }
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
    ////////////////////////////////
    /// Restart the db instance...
    ////////////////////////////////
    //    system(tree_cmd.c_str());
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = WalReplayTest::config_path();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();

        {
            auto txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kNormal);
            Vector<ColumnID> column_ids{0};
            auto [table_entry, status] = txn->GetTableByName("default_db", "test_hnsw");
            EXPECT_NE(table_entry, nullptr);

            auto table_index_meta = table_entry->GetIndexMetaPtrByName("hnsw_index");

            EXPECT_NE(table_index_meta, nullptr);
            EXPECT_EQ(*table_index_meta->index_name(), "hnsw_index");
            //            EXPECT_EQ(table_index_meta->index_entry_list().size(), 1u);
            //            auto table_index_entry_front = static_cast<TableIndexEntry *>(table_index_meta->index_entry_list().front().get());
            //            EXPECT_EQ(*table_index_entry_front->index_base()->index_name_, "hnsw_index");
            txn_mgr->CommitTxn(txn);
        }

        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
}
