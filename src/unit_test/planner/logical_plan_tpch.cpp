
#if 0

#include "bin/compilation_config.h"
#include "common/column_vector/column_vector.h"
#include "common/types/value.h"
#include "executor/physical_planner.h"
#include "legacy_sched/operator_pipeline.h"
#include "main/hybridsearch.h"
#include "main/logger.h"
#include "main/profiler/show_logical_plan.h"
#include "main/session.h"
#include "parser/sql_parser.h"
#include "planner/logical_planner.h"
#include "planner/optimizer.h"
#include "test_helper/sql_runner.h"

#include "gtest/gtest.h"
import base_test;


using namespace hybridsearch;
class LogicalPlannerTpchTest : public BaseTest {};

TEST_F(LogicalPlannerTpchTest, test1) {
    using namespace hybridsearch;

    // Create tables;
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/create.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Show tables
    {
        SQLRunner::Run("show tables;", false);
    }

    // Insert CUSTOMER table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_customer.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }
//
    // Insert LINEITEM table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_lineitem.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }
//
    // Insert NATION table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_nation.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Insert ORDERS table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_orders.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Insert PARTS table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_parts.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Insert PARTSUPP table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_partsupp.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Insert REGION table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_region.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    // Insert SUPPLIER table
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/insert_supplier.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }

    {
        // Q1
        String sql_text = String(TEST_DATA_PATH) + "/tpch/q01.sql";
        std::ifstream t(sql_text.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
//        SQLRunner::Run(input_sql, false);
    }

//    Not executable
//    {
//        // Q3
//        String sql_text = String(TEST_DATA_PATH) + "/tpch/q03.sql";
//        std::ifstream t(sql_text.c_str());
//        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
//        SQLRunner::Run(input_sql, false);
//    }

//    {
//        // Q5
//        String sql_text = String(TEST_DATA_PATH) + "/tpch/q05.sql";
//        std::ifstream t(sql_text.c_str());
//        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
//        SQLRunner::Run(input_sql, false);
//    }

    {
        // Q6
        String sql_text = String(TEST_DATA_PATH) + "/tpch/q06.sql";
        std::ifstream t(sql_text.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
//        SQLRunner::Run(input_sql, false);
    }

//    {
//        // Q10
//        String sql_text = String(TEST_DATA_PATH) + "/tpch/q10.sql";
//        std::ifstream t(sql_text.c_str());
//        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
//        SQLRunner::Run(input_sql, false);
//    }

    // DROP tables;
    {
        String ddl_sql = String(TEST_DATA_PATH) + "/tpch/drop.sql";
        std::ifstream t(ddl_sql.c_str());
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        SQLRunner::Run(input_sql, false);
    }
}

#endif
