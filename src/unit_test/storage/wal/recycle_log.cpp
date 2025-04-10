

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
import txn_state;

using namespace hybridsearch;

class RecycleLogTest : public BaseTestParamStr {
protected:
    static std::shared_ptr<std::string> test_ckp_recycle_config() {
        return GetParam() == BaseTestParamStr::NULL_CONFIG_PATH
                   ? std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp.toml")
                   : std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_ckp_vfs_off.toml");
    }

    void SetUp() override { CleanupDbDirs(); }

    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams,
                         RecycleLogTest,
                         ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH));

TEST_P(RecycleLogTest, recycle_wal_after_delta_checkpoint) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = RecycleLogTest::test_ckp_recycle_config();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        Config *config = storage->config();
        TxnManager *txn_mgr = storage->txn_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        const String &wal_dir = config->WALDir();
        {
            time_t start = time(nullptr);
            while (true) {
                time_t now = time(nullptr);
                if (now - start > 20) {
                    String error_message = "Timeout";
                    UnrecoverableError(error_message);
                }
                // create and drop db to fill wal log
                {
                    auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop db"), TransactionType::kNormal);
                    auto status = txn->DropDatabase("db1", ConflictType::kIgnore);
                    ASSERT_TRUE(status.ok() || status.code() == ErrorCode::kIgnore);
                    txn_mgr->CommitTxn(txn);
                }
                { // put create after drop to prevent the merge delta result is empty
                    auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
                    auto status = txn->CreateDatabase(MakeShared<String>("db1"), ConflictType::kIgnore, MakeShared<String>());
                    ASSERT_TRUE(status.ok());
                    txn_mgr->CommitTxn(txn);
                }
                { // loop until the wal directory has a non-temp wal
                    auto [temp_wal_info, wal_infos] = WalFile::ParseWalFilenames(wal_dir);
                    if (wal_infos.size() > 0) {
                        break;
                    }
                }
            }
        }
        TxnTimeStamp ckp_commit_ts = 0;
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false /*full_check_point*/);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            ckp_commit_ts = txn_mgr->CommitTxn(txn);
        }
        {
            // assert there is one log file
            auto [temp_wal_file, wal_files] = WalFile::ParseWalFilenames(wal_dir);
            ASSERT_TRUE(temp_wal_file.has_value());
            if (wal_files.size() == 1) {
                ASSERT_EQ(ckp_commit_ts, wal_files[0].max_commit_ts_);
            } else {
                ASSERT_TRUE(wal_files.empty());
            }
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
        hybridsearch::GlobalResourceUsage::Init(); // test replay
#endif
        std::shared_ptr<std::string> config_path = RecycleLogTest::test_ckp_recycle_config();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);
            Status status = txn->GetDatabase("db1");
            ASSERT_TRUE(status.ok());
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

TEST_P(RecycleLogTest, recycle_wal_after_full_checkpoint) {
    {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = RecycleLogTest::test_ckp_recycle_config();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        Config *config = storage->config();
        TxnManager *txn_mgr = storage->txn_manager();
        BGTaskProcessor *bg_processor = storage->bg_processor();

        const String &wal_dir = config->WALDir();
        const String &catalog_dir = config->DataDir() + "/" + String(CATALOG_FILE_DIR);
        for (int i = 0; i < 2; ++i) { // create 2 delta catalog file
            time_t start = time(nullptr);
            while (true) {
                time_t now = time(nullptr);
                if (now - start > 20) {
                    String error_message = "Timeout";
                    UnrecoverableError(error_message);
                }
                // create and drop db to fill wal log
                {
                    auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("drop db"), TransactionType::kNormal);
                    auto status = txn->DropDatabase("db1", ConflictType::kIgnore);
                    ASSERT_TRUE(status.ok() || status.code() == ErrorCode::kIgnore);
                    txn_mgr->CommitTxn(txn);
                }
                { // put create after drop to prevent the merge delta result is empty
                    auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("create db"), TransactionType::kNormal);
                    auto status = txn->CreateDatabase(MakeShared<String>("db1"), ConflictType::kIgnore, MakeShared<String>());
                    ASSERT_TRUE(status.ok());
                    txn_mgr->CommitTxn(txn);
                }
                { // loop until the wal directory has a non-temp wal
                    auto [temp_wal_info, wal_infos] = WalFile::ParseWalFilenames(wal_dir);
                    if (wal_infos.size() > 0) {
                        break;
                    }
                }
            }
            {
                auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("delta ckp"), TransactionType::kCheckpoint);
                SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, false /*delta_check_point*/);
                bg_processor->Submit(force_ckp_task);
                force_ckp_task->Wait();
                txn_mgr->CommitTxn(txn);
            }
        }
        {
            auto [full_catalog_infos, delta_catalog_infos, temp_full_infos, temp_delta_infos] = CatalogFile::ParseCheckpointFilenames(catalog_dir);
            ASSERT_EQ(full_catalog_infos.size(), 1ul); // initialize will write a full checkpoint
            ASSERT_EQ(delta_catalog_infos.size(), 2ul);
            ASSERT_TRUE(temp_full_infos.empty());
            ASSERT_TRUE(temp_delta_infos.empty());
        }
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("check index"), TransactionType::kCheckpoint);
            SharedPtr<ForceCheckpointTask> force_ckp_task = MakeShared<ForceCheckpointTask>(txn, true /*full_check_point*/);
            bg_processor->Submit(force_ckp_task);
            force_ckp_task->Wait();
            txn_mgr->CommitTxn(txn);
        }
        {
            // assert there is one full catalog file
            auto [full_catalog_infos, delta_catalog_infos, temp_full_infos, temp_delta_infos] = CatalogFile::ParseCheckpointFilenames(catalog_dir);
            ASSERT_EQ(full_catalog_infos.size(), 1ul);
            ASSERT_TRUE(delta_catalog_infos.empty());
            ASSERT_TRUE(temp_full_infos.empty());
            ASSERT_TRUE(temp_delta_infos.empty());
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
        hybridsearch::GlobalResourceUsage::Init(); // test replay
#endif
        std::shared_ptr<std::string> config_path = RecycleLogTest::test_ckp_recycle_config();
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        Storage *storage = hybridsearch::hybridsearchContext::instance().storage();
        TxnManager *txn_mgr = storage->txn_manager();
        {
            auto *txn = txn_mgr->BeginTxn(MakeUnique<String>("get db"), TransactionType::kRead);
            Status status = txn->GetDatabase("db1");
            ASSERT_TRUE(status.ok());
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
