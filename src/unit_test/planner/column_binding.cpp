

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;
import column_binding;

using namespace hybridsearch;
class ColumnBindingTest : public BaseTest {};

TEST_F(ColumnBindingTest, test1) {
    using namespace hybridsearch;

    ColumnBinding column1(0, 1);
    ColumnBinding column2(0, 1);
    ColumnBinding column3(1, 2);

    EXPECT_TRUE(column1 == column2);
    EXPECT_TRUE(column1 != column3);
}
