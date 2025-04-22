

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;

import function_set;
import aggregate_function_set;
import aggregate_function;
import function;
import column_expression;
import value;
import default_values;
import data_block;
import cast_table;
import column_vector;
import logical_type;

using namespace hybridsearch;
class CastTableTest : public BaseTest {};

TEST_F(CastTableTest, casttable_boolean) {
    using namespace hybridsearch;
    for (i8 to = to_underlying_val(LogicalType::kBoolean); to < to_underlying_val(LogicalType::kInvalid); ++to) {
        switch (to) {
            case to_underlying_val(LogicalType::kBoolean): {
                EXPECT_EQ(CastTable::instance().GetCastCost(LogicalType::kBoolean, LogicalType::kBoolean), 0);
                break;
            }
            case to_underlying_val(LogicalType::kVarchar): {
                EXPECT_EQ(CastTable::instance().GetCastCost(LogicalType::kBoolean, LogicalType::kVarchar), 100);
                break;
            }
            default: {
                EXPECT_EQ(CastTable::instance().GetCastCost(LogicalType::kBoolean, static_cast<LogicalType>(to)), -1);
            }
        }
    }
}
