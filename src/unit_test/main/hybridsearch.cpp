

#include "statement/command_statement.h"
#include "gtest/gtest.h"
import base_test;

import stl;
import hybridsearch;
import query_result;
import data_block;
import value;
import query_options;

import logical_type;
import internal_types;
import parsed_expr;
import constant_expr;
import search_expr;
import column_expr;
import insert_row_expr;
import column_def;
import data_type;

using namespace hybridsearch;
class hybridsearchTest : public BaseTest {};

TEST_F(hybridsearchTest, test1) {
    using namespace hybridsearch;
    String path = GetHomeDir();
    RemoveDbDirs();
    hybridsearch::LocalInit(path);

    SharedPtr<hybridsearch> hybridsearch = hybridsearch::LocalConnect();

    {
        QueryResult result = hybridsearch->ListDatabases();
        EXPECT_EQ(result.result_table_->row_count(), 1);
        EXPECT_EQ(result.result_table_->ColumnCount(), 3u);
        EXPECT_EQ(result.result_table_->GetColumnNameById(0), "database");
        EXPECT_EQ(result.result_table_->DataBlockCount(), 1u);
        SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 1u);
        Value value = data_block->GetValue(0, 0);
        const String &s2 = value.GetVarchar();
        EXPECT_STREQ(s2.c_str(), "default_db");
    }

    {
        CreateDatabaseOptions create_db_opts;
        hybridsearch->CreateDatabase("db1", create_db_opts, "");
        QueryResult result = hybridsearch->ListDatabases();
        SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 2);
        Value value = data_block->GetValue(0, 0);
        const String &s2 = value.GetVarchar();
        EXPECT_STREQ(s2.c_str(), "db1");

        value = data_block->GetValue(0, 1);
        const String &s3 = value.GetVarchar();
        EXPECT_STREQ(s3.c_str(), "default_db");

        result = hybridsearch->GetDatabase("db1");
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->GetDatabase("db2");
        EXPECT_FALSE(result.IsOk());

        DropDatabaseOptions drop_db_opts;
        result = hybridsearch->DropDatabase("db1", drop_db_opts);
        EXPECT_FALSE(result.IsOk());

        result = hybridsearch->GetDatabase("default_db");
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->DropDatabase("db1", drop_db_opts);
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->ListDatabases();
        data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 1);
        value = data_block->GetValue(0, 0);
        const String &s4 = value.GetVarchar();
        EXPECT_STREQ(s4.c_str(), "default_db");
        result = hybridsearch->GetDatabase("default_db");
        EXPECT_TRUE(result.IsOk());
    }

    {
        QueryResult result = hybridsearch->GetDatabase("default_db");
        EXPECT_TRUE(result.IsOk());

        CreateTableOptions create_table_opts;

        SizeT column_count = 2;
        //        Vector<SharedPtr<ColumnDef>> columns;
        Vector<ColumnDef *> column_defs;
        column_defs.reserve(column_count);

        SharedPtr<DataType> col_type = MakeShared<DataType>(LogicalType::kBoolean);
        String col_name = "col1";
        auto col_def = new ColumnDef(0, col_type, col_name, std::set<ConstraintType>());
        column_defs.emplace_back(col_def);

        col_type = MakeShared<DataType>(LogicalType::kBigInt);
        col_name = "col2";
        col_def = new ColumnDef(1, col_type, col_name, std::set<ConstraintType>());
        column_defs.emplace_back(col_def);

        result = hybridsearch->CreateTable("default_db", "table1", column_defs, Vector<TableConstraint *>(), create_table_opts);
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->ListTables("default_db");
        SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 1);
        Value value = data_block->GetValue(1, 0);
        const String &s2 = value.GetVarchar();
        EXPECT_STREQ(s2.c_str(), "table1");

        result = hybridsearch->GetTable("default_db", "table1");
        EXPECT_TRUE(result.IsOk());

        DropTableOptions drop_table_opts;
        result = hybridsearch->DropTable("default_db", "table1", drop_table_opts);
        EXPECT_TRUE(result.IsOk());
        result = hybridsearch->ListTables("default_db");
        data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 0);

        result = hybridsearch->GetTable("default_db", "table1");
        EXPECT_FALSE(result.IsOk());
    }

    {
        QueryResult result = hybridsearch->GetDatabase("default_db");
        EXPECT_TRUE(result.IsOk());

        CreateTableOptions create_table_opts;

        SizeT column_count = 1;
        Vector<ColumnDef *> column_defs;
        column_defs.reserve(column_count);

        SharedPtr<DataType> col_type = MakeShared<DataType>(LogicalType::kBigInt);
        String col1_name = "col1";
        auto col_def = new ColumnDef(0, col_type, col1_name, std::set<ConstraintType>());
        column_defs.emplace_back(col_def);

        col_type = MakeShared<DataType>(LogicalType::kSmallInt);
        String col2_name = "col2";
        col_def = new ColumnDef(1, col_type, col2_name, std::set<ConstraintType>());
        column_defs.emplace_back(col_def);

        result = hybridsearch->CreateTable("default_db", "table1", column_defs, Vector<TableConstraint *>(), create_table_opts);
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->GetTable("default_db", "table1");
        EXPECT_TRUE(result.IsOk());

        //        Vector<String> *columns, Vector<Vector<ParsedExpr *> *> *values

        Vector<String> columns = {col1_name, col2_name};
        Vector<UniquePtr<ParsedExpr>> values{};
        auto value1 = MakeUnique<ConstantExpr>(LiteralType::kInteger);
        value1->integer_value_ = 11;
        values.emplace_back(std::move(value1));
        auto value2 = MakeUnique<ConstantExpr>(LiteralType::kInteger);
        value2->integer_value_ = 22;
        values.emplace_back(std::move(value2));
        auto insert_row = new InsertRowExpr();
        insert_row->columns_ = std::move(columns);
        insert_row->values_ = std::move(values);
        Vector<InsertRowExpr *> *insert_rows = new Vector<InsertRowExpr *>();
        insert_rows->emplace_back(insert_row);
        hybridsearch->Insert("default_db", "table1", insert_rows);

        //        QueryResult Search(Vector<Pair<ParsedExpr *, ParsedExpr *>> &vector_expr,
        //                           Vector<Pair<ParsedExpr *, ParsedExpr *>> &fts_expr,
        //                           ParsedExpr *filter,
        //                           Vector<ParsedExpr *> *output_columns,
        //                           ParsedExpr *offset,
        //                           ParsedExpr *limit);

        Vector<ParsedExpr *> *output_columns = new Vector<ParsedExpr *>();
        ColumnExpr *col1 = new ColumnExpr();
        col1->names_.emplace_back(col1_name);
        output_columns->emplace_back(col1);

        ColumnExpr *col2 = new ColumnExpr();
        col2->names_.emplace_back(col2_name);
        output_columns->emplace_back(col2);

        SearchExpr *search_expr = nullptr;

        result = hybridsearch->Search("default_db", "table1", search_expr, nullptr, nullptr, nullptr, output_columns, nullptr, nullptr, nullptr, nullptr, false);
        SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 1);
        Value value = data_block->GetValue(0, 0);
        EXPECT_EQ(value.type().type(), LogicalType::kBigInt);
        EXPECT_EQ(value.value_.big_int, 11);

        value = data_block->GetValue(1, 0);
        EXPECT_EQ(value.type().type(), LogicalType::kSmallInt);
        EXPECT_EQ(value.value_.big_int, 22);

        DropTableOptions drop_table_opts;
        result = hybridsearch->DropTable("default_db", "table1", drop_table_opts);
        EXPECT_TRUE(result.IsOk());
    }

    {
        hybridsearch->Query("create database db1;");
        QueryResult result = hybridsearch->Query("show databases;");
        SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 2);
        Value value = data_block->GetValue(0, 0);
        const String &s2 = value.GetVarchar();
        EXPECT_STREQ(s2.c_str(), "db1");

        value = data_block->GetValue(0, 1);
        const String &s3 = value.GetVarchar();
        EXPECT_STREQ(s3.c_str(), "default_db");

        result = hybridsearch->Query("drop database db1");
        EXPECT_TRUE(result.IsOk());

        result = hybridsearch->ListDatabases();
        data_block = result.result_table_->GetDataBlockById(0);
        EXPECT_EQ(data_block->row_count(), 1);
        value = data_block->GetValue(0, 0);
        const String &s4 = value.GetVarchar();
        EXPECT_STREQ(s4.c_str(), "default_db");
        result = hybridsearch->GetDatabase("default_db");
        EXPECT_TRUE(result.IsOk());
    }
    hybridsearch->LocalDisconnect();

    hybridsearch::LocalUnInit();
}

TEST_F(hybridsearchTest, test2) {
    using namespace hybridsearch;
    String path = GetHomeDir();
    RemoveDbDirs();
    hybridsearch::LocalInit(path);

    SharedPtr<hybridsearch> hybridsearch = hybridsearch::LocalConnect();

    {
        QueryResult result = hybridsearch->ShowVariable("total_commit_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("active_txn_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("total_rollback_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("buffer_object_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("unused_buffer_object", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("next_transaction_id", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("delta_log_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("schedule_policy", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("active_wal_filename", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("current_timestamp", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("session_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("query_count", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("buffer_usage", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("error", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), false);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("connected_timestamp", SetScope::kSession);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("profile_record_capacity", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("total_rollback_count", SetScope::kSession);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("profile", SetScope::kGlobal);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("total_commit_count", SetScope::kSession);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("query_count", SetScope::kSession);
        EXPECT_EQ(result.IsOk(), true);
    }

    {
        QueryResult result = hybridsearch->ShowVariable("error", SetScope::kSession);
        EXPECT_EQ(result.IsOk(), false);
    }

    hybridsearch->LocalDisconnect();

    hybridsearch::LocalUnInit();
}