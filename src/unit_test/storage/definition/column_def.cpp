

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import stl;
import global_resource_usage;
import third_party;
import logger;

import hybridsearch_context;
import internal_types;
import logical_type;
import column_def;
import data_type;

using namespace hybridsearch;
class ColumnDefTest : public BaseTest {};

TEST_F(ColumnDefTest, test1) {
    using namespace hybridsearch;

    std::set<ConstraintType> constraints;
    constraints.insert(ConstraintType::kUnique);
    constraints.insert(ConstraintType::kNotNull);
    auto column_def_ptr = MakeShared<ColumnDef>(0, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "c1", constraints);
    EXPECT_EQ(*column_def_ptr->type(), DataType(LogicalType::kTinyInt));
    EXPECT_EQ(column_def_ptr->id(), 0);
    EXPECT_STREQ(column_def_ptr->name().c_str(), "c1");
    EXPECT_EQ(column_def_ptr->ToString(), "c1 TinyInt Not nullable Unique default Null");
}

TEST_F(ColumnDefTest, test2) {
    using namespace hybridsearch;

    std::set<ConstraintType> constraints;
    constraints.insert(ConstraintType::kPrimaryKey);
    auto column_def_ptr = MakeShared<ColumnDef>(3, MakeShared<DataType>(LogicalType::kVarchar), "c2", constraints);
    EXPECT_EQ(*column_def_ptr->type(), DataType(LogicalType::kVarchar));
    EXPECT_EQ(column_def_ptr->id(), 3);
    EXPECT_EQ(column_def_ptr->name(), "c2");
    EXPECT_EQ(column_def_ptr->ToString(), "c2 Varchar PrimaryKey default Null");
}
