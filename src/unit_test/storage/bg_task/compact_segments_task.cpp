

#include <numeric>
#include <thread>

#include "gtest/gtest.h"
import base_test;

import stl;
import storage;
import global_resource_usage;
import hybridsearch_context;
import status;
import catalog;
import txn;
import buffer_manager;
import txn_manager;
import column_vector;

import table_def;
import value;
import physical_import;
import default_values;
import hybridsearch_exception;
import base_table_ref;
import logical_type;
import internal_types;
import extra_ddl_info;
import column_def;
import data_type;
import segment_entry;
import block_entry;
import compaction_process;
import compilation_config;
import logger;
import third_party;
import txn_state;

using namespace hybridsearch;

class CompactTaskTest : public BaseTestParamStr {
protected:
    void AddSegments(TxnManager *txn_mgr, const String &table_name, const Vector<SizeT> &segment_sizes, BufferManager *buffer_mgr) {
        for (SizeT segment_size : segment_sizes) {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("import table"), TransactionType::kNormal);

            auto [table_info, status] = txn->GetTableInfo("default_db", table_name);
            auto column_count = table_info->column_count_;

            auto [segment_entry, segment_status] = txn->MakeNewSegment("default_db", table_name);
            if (!segment_status.ok()) {
                RecoverableError(segment_status);
            }

            auto block_entry = BlockEntry::NewBlockEntry(segment_entry.get(), 0, 0, column_count, txn);

            while (segment_size > 0) {
                SizeT write_size = std::min(SizeT(DEFAULT_BLOCK_CAPACITY), segment_size);
                segment_size -= write_size;
                Vector<ColumnVector> column_vectors;
                {
                    auto column_vector = ColumnVector(MakeShared<DataType>(LogicalType::kTinyInt));
                    column_vector.Initialize();
                    Value v = Value::MakeTinyInt(static_cast<TinyIntT>(1));
                    for (int i = 0; i < (int)write_size; ++i) {
                        column_vector.AppendValue(v);
                    }
                    column_vectors.push_back(std::move(column_vector));
                }
                block_entry->AppendBlock(column_vectors, 0, write_size, buffer_mgr);
                segment_entry->AppendBlockEntry(std::move(block_entry));
                block_entry = BlockEntry::NewBlockEntry(segment_entry.get(), segment_entry->GetNextBlockID(), 0, 1, txn);
            }
            PhysicalImport::SaveSegmentData(table_info.get(), txn, segment_entry);
            txn_mgr->CommitTxn(txn);
        }
    }
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         CompactTaskTest,
                         ::testing::Values((std::string(test_data_path()) + "/config/test_close_bgtask.toml").c_str(),
                                           (std::string(test_data_path()) + "/config/test_close_bgtask_vfs_off.toml").c_str()));

class SilentLogTestCompactTaskTest : public CompactTaskTest {
    void SetUp() override {
        RemoveDbDirs();
        system(("mkdir -p " + String(GetFullPersistDir())).c_str());
        system(("mkdir -p " + String(GetFullDataDir())).c_str());
        system(("mkdir -p " + String(GetFullDataDir())).c_str());
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::string config_path_str = GetParam();
        std::shared_ptr<std::string> config_path = nullptr;
        if (config_path_str != BaseTestParamStr::NULL_CONFIG_PATH) {
            config_path = hybridsearch::MakeShared<std::string>(config_path_str);
        }
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();
    }
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         SilentLogTestCompactTaskTest,
                         ::testing::Values((std::string(test_data_path()) + "/config/test_close_bgtask_silent.toml").c_str(),
                                           (std::string(test_data_path()) + "/config/test_close_bgtask_silent_vfs_off.toml").c_str()));

TEST_P(CompactTaskTest, compact_to_single_segment) {
    {
        String table_name = "tbl1";

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        BufferManager *buffer_manager = storage->buffer_manager();
        TxnManager *txn_mgr = storage->txn_manager();
        CompactionProcessor *compaction_process = storage->compaction_processor();

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
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        Vector<SizeT> segment_sizes{1, 10, 100, 1000, 10000, 100000};
        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_manager);

        { // add compact
            auto commit_ts = compaction_process->ManualDoCompact("default_db", table_name, false);
            EXPECT_NE(commit_ts, 0u);
        }

        {
            auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn5->BeginTS();
            auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
            EXPECT_NE(table_entry, nullptr);

            size_t test_segment_n = segment_sizes.size();
            size_t row_count = std::accumulate(segment_sizes.begin(), segment_sizes.end(), 0);

            for (size_t i = 0; i < test_segment_n; ++i) {
                auto segment_entry = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_NE(segment_entry, nullptr);
                EXPECT_EQ(segment_entry->status(), SegmentStatus::kDeprecated);
            }
            auto compact_segment = table_entry->GetSegmentByID(test_segment_n, begin_ts);
            EXPECT_NE(compact_segment, nullptr);
            EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);
            EXPECT_EQ(compact_segment->actual_row_count(), row_count);

            txn_mgr->CommitTxn(txn5);
        }
    }
}

TEST_P(CompactTaskTest, compact_to_two_segment) {
    GTEST_SKIP() << "Skipping slow test.";
    {
        String table_name = "tbl1";

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        BufferManager *buffer_manager = storage->buffer_manager();
        TxnManager *txn_mgr = storage->txn_manager();
        CompactionProcessor *compaction_process = storage->compaction_processor();

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
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        Vector<SizeT> segment_sizes{1, 10, 100, 1000, 10000, 100000, 1000000, 2000000, 4000000, 8000000};
        int row_count = std::accumulate(segment_sizes.begin(), segment_sizes.end(), 0);

        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_manager);

        {
            auto commit_ts = compaction_process->ManualDoCompact("default_db", table_name, false);
            EXPECT_NE(commit_ts, 0u);
        }
        {
            auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn5->BeginTS();
            auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
            EXPECT_NE(table_entry, nullptr);

            int test_segment_n = segment_sizes.size();

            for (int i = 0; i < test_segment_n; ++i) {
                auto segment_entry = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_NE(segment_entry, nullptr);
                EXPECT_EQ(segment_entry->status(), SegmentStatus::kDeprecated);
            }
            int cnt = 0;
            for (int i = test_segment_n; i < test_segment_n + 2; ++i) {
                auto compact_segment = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_NE(compact_segment, nullptr);
                EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);
                cnt += compact_segment->actual_row_count();
            }
            EXPECT_EQ(cnt, row_count);

            txn_mgr->CommitTxn(txn5);
        }
    }
}

TEST_P(CompactTaskTest, compact_with_delete) {
    GTEST_SKIP() << "Skipping slow test.";
    {
        String table_name = "tbl1";

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        BufferManager *buffer_manager = storage->buffer_manager();
        TxnManager *txn_mgr = storage->txn_manager();
        CompactionProcessor *compaction_process = storage->compaction_processor();

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
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        Vector<SizeT> segment_sizes{1, 10, 100, 1000, 10000, 100000, 1000000, 2000000, 4000000};
        int row_count = std::accumulate(segment_sizes.begin(), segment_sizes.end(), 0);

        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_manager);

        SizeT delete_n = 0;
        {
            auto txn3 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);

            Vector<RowID> delete_row_ids;
            for (int i = 0; i < (int)segment_sizes.size(); ++i) {
                int delete_n1 = segment_sizes[i] / 2;
                Vector<SegmentOffset> offsets;
                for (int j = 0; j < delete_n1; ++j) {
                    offsets.push_back(rand() % segment_sizes[i]);
                }
                std::sort(offsets.begin(), offsets.end());
                offsets.erase(std::unique(offsets.begin(), offsets.end()), offsets.end());
                for (SegmentOffset offset : offsets) {
                    delete_row_ids.emplace_back(i, offset);
                }
                delete_n += offsets.size();
            }

            auto [table_entry, status] = txn3->GetTableByName("default_db", table_name);
            EXPECT_TRUE(status.ok());
            txn3->Delete("default_db", table_name, delete_row_ids);

            txn_mgr->CommitTxn(txn3);
        }

        {
            auto commit_ts = compaction_process->ManualDoCompact("default_db", table_name, false);
            EXPECT_NE(commit_ts, 0u);
        }
        {
            auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn5->BeginTS();
            auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
            EXPECT_NE(table_entry, nullptr);

            int test_segment_n = segment_sizes.size();
            for (int i = 0; i < test_segment_n; ++i) {
                auto segment_entry = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_NE(segment_entry, nullptr);
                EXPECT_EQ(segment_entry->status(), SegmentStatus::kDeprecated);
            }
            auto compact_segment = table_entry->GetSegmentByID(test_segment_n, begin_ts);
            EXPECT_NE(compact_segment, nullptr);
            EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);

            EXPECT_EQ(compact_segment->actual_row_count(), row_count - delete_n);

            txn_mgr->CommitTxn(txn5);
        }
    }
}

TEST_P(SilentLogTestCompactTaskTest, delete_in_compact_process) {
    {
        String table_name = "tbl1";

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
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        Vector<SizeT> segment_sizes{1, 10, 100, 1000, 10000, 100000};
        SizeT row_count = std::accumulate(segment_sizes.begin(), segment_sizes.end(), 0);

        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_manager);

        int delete_n = 0;
        {
            auto txn3 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);

            Vector<RowID> delete_row_ids;
            for (int i = 0; i < (int)segment_sizes.size(); ++i) {
                int delete_n1 = segment_sizes[i] / 4;
                Vector<SegmentOffset> offsets;
                for (int j = 0; j < delete_n1; ++j) {
                    offsets.push_back(rand() % (segment_sizes[i] / 2));
                }
                std::sort(offsets.begin(), offsets.end());
                offsets.erase(std::unique(offsets.begin(), offsets.end()), offsets.end());
                for (SegmentOffset offset : offsets) {
                    delete_row_ids.emplace_back(i, offset);
                }
                delete_n += offsets.size();
            }

            auto [table_entry, status] = txn3->GetTableByName("default_db", table_name);
            EXPECT_TRUE(status.ok());
            txn3->Delete("default_db", table_name, delete_row_ids);

            txn_mgr->CommitTxn(txn3);
        }

        { // add compact
            auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);

            Vector<RowID> delete_row_ids;
            for (int i = 0; i < (int)segment_sizes.size(); ++i) {
                int delete_n2 = segment_sizes[i] / 4;
                Vector<SegmentOffset> offsets;
                for (int j = 0; j < delete_n2; ++j) {
                    offsets.push_back(rand() % (segment_sizes[i] - segment_sizes[i] / 2) + segment_sizes[i] / 2);
                }
                std::sort(offsets.begin(), offsets.end());
                offsets.erase(std::unique(offsets.begin(), offsets.end()), offsets.end());
                for (SegmentOffset offset : offsets) {
                    delete_row_ids.emplace_back(i, offset);
                }
                delete_n += offsets.size();
            }

            auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
            EXPECT_TRUE(status.ok());
            txn5->Delete("default_db", table_name, delete_row_ids);

            Thread t([&]() {
                auto commit_ts = compaction_processor->ManualDoCompact("default_db", table_name, false);
                EXPECT_NE(commit_ts, 0u);
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            txn_mgr->CommitTxn(txn5);
            row_count -= delete_n;
            t.join();
        }
        {
            auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
            TxnTimeStamp begin_ts = txn5->BeginTS();
            auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
            EXPECT_NE(table_entry, nullptr);

            int test_segment_n = segment_sizes.size();
            for (int i = 0; i < test_segment_n; ++i) {
                auto segment_entry = table_entry->GetSegmentByID(i, begin_ts);
                EXPECT_NE(segment_entry, nullptr);
                EXPECT_EQ(segment_entry->status(), SegmentStatus::kDeprecated);
            }
            auto compact_segment = table_entry->GetSegmentByID(test_segment_n, begin_ts);
            EXPECT_NE(compact_segment, nullptr);
            EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);

            EXPECT_EQ(compact_segment->actual_row_count(), row_count);

            txn_mgr->CommitTxn(txn5);
        }
    }
}

TEST_P(CompactTaskTest, uncommit_delete_in_compact_process) {
    for (int task_i = 0; task_i < 10; ++task_i) {
        LOG_INFO(fmt::format("Test {}", task_i));
        String table_name = fmt::format("tbl{}", task_i);

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
        { // create table
            auto tbl1_def = MakeUnique<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

            Status status = txn->CreateTable("default_db", std::move(tbl1_def), ConflictType::kIgnore);
            EXPECT_TRUE(status.ok());

            txn_mgr->CommitTxn(txn);
        }
        Vector<SizeT> segment_sizes{1, 10, 100, 1000, 10000, 100000};
        int row_count = std::accumulate(segment_sizes.begin(), segment_sizes.end(), 0);

        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_manager);

        SizeT delete_n = 0;
        {
            auto txn3 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);

            Vector<RowID> delete_row_ids;
            for (int i = 0; i < (int)segment_sizes.size(); ++i) {
                int delete_n1 = segment_sizes[i] / 6;
                Vector<SegmentOffset> offsets;
                for (int j = 0; j < delete_n1; ++j) {
                    offsets.push_back(rand() % (segment_sizes[i] / 3));
                }
                std::sort(offsets.begin(), offsets.end());
                offsets.erase(std::unique(offsets.begin(), offsets.end()), offsets.end());
                for (SegmentOffset offset : offsets) {
                    delete_row_ids.emplace_back(i, offset);
                }
                delete_n += offsets.size();
            }
            LOG_INFO(fmt::format("A: delete_n: {}", delete_n));
            auto [table_entry, status] = txn3->GetTableByName("default_db", table_name);
            int total_row_n = table_entry->row_count();
            EXPECT_EQ(total_row_n, row_count);
            EXPECT_TRUE(status.ok());
            txn3->Delete("default_db", table_name, delete_row_ids);

            txn_mgr->CommitTxn(txn3);
        }

        // add compact
        {
            Vector<RowID> delete_row_ids;
            Vector<RowID> delete_row_ids2;

            int delete_row_n1 = 0;
            int delete_row_n2 = 0;

            for (int i = 0; i < (int)segment_sizes.size(); ++i) {
                Vector<SegmentOffset> offsets;
                Vector<SegmentOffset> offsets2;
                for (int j = 0; j < (int)(segment_sizes[i] / 6); ++j) {
                    offsets.push_back(rand() % (segment_sizes[i] / 3) + segment_sizes[i] / 3);
                    offsets2.push_back(rand() % (segment_sizes[i] - segment_sizes[i] / 3 * 2) + segment_sizes[i] / 3 * 2);
                }
                std::sort(offsets.begin(), offsets.end());
                std::sort(offsets2.begin(), offsets2.end());
                offsets.erase(std::unique(offsets.begin(), offsets.end()), offsets.end());
                offsets2.erase(std::unique(offsets2.begin(), offsets2.end()), offsets2.end());
                for (SegmentOffset offset : offsets) {
                    delete_row_ids.emplace_back(i, offset);
                }
                for (SegmentOffset offset2 : offsets2) {
                    delete_row_ids2.emplace_back(i, offset2);
                }

                delete_row_n1 += offsets.size();
                delete_row_n2 += offsets2.size();

                LOG_INFO(fmt::format("Delete1 {} in segment {}", offsets.size(), i));
            }

            auto commit_ts = compaction_processor->ManualDoCompact("default_db", table_name, false, [&]() {
                auto delete_txn1 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);
                LOG_INFO(fmt::format("delete1 txn id: {}", delete_txn1->TxnID()));
                auto [table_entry, status] = delete_txn1->GetTableByName("default_db", table_name);
                EXPECT_TRUE(status.ok());

                try {
                    delete_txn1->Delete("default_db", table_name, delete_row_ids);
                    txn_mgr->CommitTxn(delete_txn1);
                    LOG_INFO(fmt::format("Delete 1 is committed, {}", delete_row_n1));
                    delete_n += delete_row_n1;
                } catch (const RecoverableException &e) {
                    LOG_INFO("Delete 1 is row backed");
                }
            });
            EXPECT_NE(commit_ts, 0u);
            {
                auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("delete table"), TransactionType::kNormal);
                try {
                    auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
                    EXPECT_TRUE(status.ok());
                    txn5->Delete("default_db", table_name, delete_row_ids2);
                    ASSERT_EQ(0, 1);
                } catch (const RecoverableException &e) {
                    EXPECT_EQ(e.ErrorCode(), ErrorCode::kTxnRollback);
                }
                txn_mgr->RollBackTxn(txn5);
            }

            {
                auto txn5 = txn_mgr->BeginTxn(MakeUnique<String>("check table"), TransactionType::kNormal);
                TxnTimeStamp begin_ts = txn5->BeginTS();
                auto [table_entry, status] = txn5->GetTableByName("default_db", table_name);
                EXPECT_NE(table_entry, nullptr);

                int test_segment_n = segment_sizes.size();
                for (int i = 0; i < test_segment_n; ++i) {
                    auto segment_entry = table_entry->GetSegmentByID(i, begin_ts);
                    EXPECT_NE(segment_entry, nullptr);
                    EXPECT_EQ(segment_entry->status(), SegmentStatus::kDeprecated);
                }
                auto compact_segment = table_entry->GetSegmentByID(test_segment_n, begin_ts);
                EXPECT_NE(compact_segment, nullptr);
                EXPECT_NE(compact_segment->status(), SegmentStatus::kDeprecated);

                if (compact_segment->actual_row_count() != row_count - delete_n) {
                    LOG_ERROR(fmt::format("compact_segment->actual_row_count():{}, row_count: {}, delete_n: {}",
                                          compact_segment->actual_row_count(),
                                          row_count,
                                          delete_n));
                    LOG_ERROR(fmt::format("delete_row_n1: {}, delete_row_n2: {}, row_n: {}", delete_row_n1, delete_row_n2, row_count));
                }
                ASSERT_EQ(compact_segment->actual_row_count(), row_count - delete_n);

                txn_mgr->CommitTxn(txn5);
            }
        }
    }
}

TEST_P(CompactTaskTest, compact_not_exist_table) {
    Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
    BufferManager *buffer_mgr = storage->buffer_manager();
    TxnManager *txn_mgr = storage->txn_manager();
    CompactionProcessor *compaction_process = storage->compaction_processor();

    String table_name = "tb1";
    SharedPtr<TableDef> tbl1_def = nullptr;
    {
        Vector<SharedPtr<ColumnDef>> columns;
        i64 column_id = 0;
        std::set<ConstraintType> constraints;
        auto column_def_ptr = MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "tiny_int_col", constraints);
        columns.emplace_back(column_def_ptr);
        tbl1_def = MakeShared<TableDef>(MakeShared<String>("default_db"), MakeShared<String>(table_name), MakeShared<String>(), columns);
    }
    {
        // create table
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

        Status status = txn->CreateTable("default_db", tbl1_def, ConflictType::kIgnore);
        EXPECT_TRUE(status.ok());

        txn_mgr->CommitTxn(txn);
    }
    {
        Vector<SizeT> segment_sizes{1, 10, 100};
        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_mgr);
    }
    {
        { // drop tb1
            auto drop_txn = txn_mgr->BeginTxn(MakeUnique<String>("drop table"), TransactionType::kNormal);
            auto status = drop_txn->DropTableCollectionByName("default_db", table_name, ConflictType::kError);
            ASSERT_TRUE(status.ok());
            txn_mgr->CommitTxn(drop_txn);
        }

        auto commit_ts = compaction_process->ManualDoCompact("default_db", table_name, false);
        EXPECT_EQ(commit_ts, 0u);
    }

    //------------------------------------------

    {
        // create table
        auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("compact table"), TransactionType::kNormal);

        Status status = txn->CreateTable("default_db", tbl1_def, ConflictType::kIgnore);
        EXPECT_TRUE(status.ok());

        txn_mgr->CommitTxn(txn);
    }
    {
        Vector<SizeT> segment_sizes{1, 10, 100};
        this->AddSegments(txn_mgr, table_name, segment_sizes, buffer_mgr);
    }
    {
        auto commit_ts = compaction_process->ManualDoCompact("default_db", table_name, false, [&]() {
            { // drop tb1
                auto drop_txn = txn_mgr->BeginTxn(MakeUnique<String>("drop table"), TransactionType::kNormal);
                auto status = drop_txn->DropTableCollectionByName("default_db", table_name, ConflictType::kError);
                ASSERT_TRUE(status.ok());
                txn_mgr->CommitTxn(drop_txn);
            }
            { // create table with same name
                auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create table"), TransactionType::kNormal);

                Status status = txn->CreateTable("default_db", tbl1_def, ConflictType::kIgnore);
                EXPECT_TRUE(status.ok());

                txn_mgr->CommitTxn(txn);
            }
        });
        EXPECT_EQ(commit_ts, 0u);
    }
}