

#include "type/complex/row_id.h"
#include "gtest/gtest.h"
import base_test;

import stl;
import compilation_config;
import hybridsearch_context;
import table_def;
import column_def;
import data_type;
import logical_type;
import storage;
import txn_manager;
import txn;
import extra_ddl_info;
import column_vector;
import data_block;
import value;
import hybridsearch_exception;
import status;
import txn_state;

using namespace hybridsearch;

class ConflictCheckTest : public BaseTest {

protected:
    void SetUp() override {
        RemoveDbDirs();

        auto config_path = std::make_shared<std::string>(std::string(test_data_path()) + "/config/test_close_bgtask.toml");
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();

        storage_ = hybridsearchContext::instance().storage();
        txn_mgr_ = storage_->txn_manager();
    }

    void TearDown() override { hybridsearch::hybridsearchContext::instance().UnInit(); }

    Txn *DeleteRow(const String &db_name, const String &table_name, Vector<SegmentOffset> segment_offsets) {
        auto *txn = txn_mgr_->BeginTxn(MakeUnique<String>("Delete row"), TransactionType::kNormal);

        Vector<RowID> row_ids;
        for (auto segment_offset : segment_offsets) {
            row_ids.push_back(RowID(0 /*segment_id*/, segment_offset));
        }

        auto [table_entry, status] = txn->GetTableByName(db_name, table_name);
        EXPECT_TRUE(status.ok());
        status = txn->Delete(db_name, table_name, row_ids, true);
        EXPECT_TRUE(status.ok());
        return txn;
    };

    void ExpectConflict(Txn *txn) {
        try {
            txn_mgr_->CommitTxn(txn);
            FAIL() << "Expected RecoverableException";
        } catch (const RecoverableException &e) {
            EXPECT_EQ(e.ErrorCode(), ErrorCode::kTxnConflict);
            txn_mgr_->RollBackTxn(txn);
        } catch (...) {
            FAIL() << "Expected RecoverableException";
        }
    };

    void InitTable(const String &db_name, const String &table_name, SharedPtr<TableDef> table_def, SizeT row_cnt) {
        auto *txn = txn_mgr_->BeginTxn(MakeUnique<String>("Init table"), TransactionType::kNormal);

        txn->CreateTable(db_name, table_def, ConflictType::kError);
        auto [table_entry, status] = txn->GetTableByName(db_name, table_name);
        EXPECT_TRUE(status.ok());

        Vector<SharedPtr<ColumnVector>> column_vectors;
        {
            auto column_vector = MakeShared<ColumnVector>(table_def->columns()[0]->type());
            column_vector->Initialize();
            for (SizeT i = 0; i < row_cnt; i++) {
                column_vector->AppendValue(Value::MakeInt(i));
            }
            column_vectors.push_back(column_vector);
        }
        auto data_block = DataBlock::Make();
        data_block->Init(column_vectors);

        status = txn->Append(db_name, table_name, data_block);
        EXPECT_TRUE(status.ok());

        txn_mgr_->CommitTxn(txn);
    }

    void CheckRowCnt(const String &db_name, const String &table_name, SizeT expected_row_cnt) {
        auto *txn = txn_mgr_->BeginTxn(MakeUnique<String>("Check row count"), TransactionType::kNormal);
        auto [table_entry, status] = txn->GetTableByName(db_name, table_name);
        EXPECT_TRUE(status.ok());

        EXPECT_EQ(table_entry->row_count(), expected_row_cnt);

        txn_mgr_->CommitTxn(txn);
    }

protected:
    Storage *storage_;
    TxnManager *txn_mgr_;
};

TEST_F(ConflictCheckTest, conflict_check_delete) {
    auto db_name = std::make_shared<std::string>("default_db");
    auto table_name = std::make_shared<std::string>("table1");
    auto column_def1 = std::make_shared<ColumnDef>(0, std::make_shared<DataType>(LogicalType::kInteger), "col1", std::set<ConstraintType>());
    auto table_def = TableDef::Make(db_name, table_name, MakeShared<String>(), {column_def1});

    SizeT row_cnt = 10;

    InitTable(*db_name, *table_name, table_def, row_cnt);
    {
        auto *txn1 = DeleteRow(*db_name, *table_name, {0});
        auto *txn2 = DeleteRow(*db_name, *table_name, {0});
        auto *txn3 = DeleteRow(*db_name, *table_name, {0});
        Vector<TransactionID> txn_ids{txn1->TxnID(), txn2->TxnID(), txn3->TxnID()};

        txn_mgr_->CommitTxn(txn1);
        ExpectConflict(txn2);
        ExpectConflict(txn3);

        --row_cnt;
        CheckRowCnt(*db_name, *table_name, row_cnt);
    }
    {
        auto *txn1 = DeleteRow(*db_name, *table_name, {1});
        auto *txn2 = DeleteRow(*db_name, *table_name, {1});
        auto *txn3 = DeleteRow(*db_name, *table_name, {1});

        txn_mgr_->CommitTxn(txn2);
        ExpectConflict(txn1);
        ExpectConflict(txn3);

        --row_cnt;
        CheckRowCnt(*db_name, *table_name, row_cnt);
    }
    {
        auto *txn1 = DeleteRow(*db_name, *table_name, {2});
        auto *txn2 = DeleteRow(*db_name, *table_name, {2});
        auto *txn3 = DeleteRow(*db_name, *table_name, {2});

        txn_mgr_->CommitTxn(txn3);
        ExpectConflict(txn2);
        ExpectConflict(txn1);

        --row_cnt;
        CheckRowCnt(*db_name, *table_name, row_cnt);
    }
    {
        auto *txn1 = DeleteRow(*db_name, *table_name, {3});
        auto *txn2 = DeleteRow(*db_name, *table_name, {3, 4});

        txn_mgr_->CommitTxn(txn1);
        ExpectConflict(txn2);

        --row_cnt;
        CheckRowCnt(*db_name, *table_name, row_cnt);
    }
    {
        auto *txn1 = DeleteRow(*db_name, *table_name, {5, 6});
        auto *txn2 = DeleteRow(*db_name, *table_name, {5});

        txn_mgr_->CommitTxn(txn1);
        ExpectConflict(txn2);

        row_cnt -= 2;
        CheckRowCnt(*db_name, *table_name, row_cnt);
    }
}
