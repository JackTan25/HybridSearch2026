

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;
import internal_types;

using namespace hybridsearch;
class PointTypeTest : public BaseTest {};

TEST_F(PointTypeTest, point1) {
    using namespace hybridsearch;

    PointT p1;
    p1.x = 1.0f;
    p1.y = 2.0f;

    PointT p2(1.0f, 2.0f);

    EXPECT_EQ(p1.x, 1.0f);
    EXPECT_EQ(p1.y, 2.0f);
    EXPECT_EQ(p2.x, 1.0f);
    EXPECT_EQ(p2.y, 2.0f);

    EXPECT_EQ(p1, p2);

    p2.Reset();

    EXPECT_EQ(p2.x, 0);
    EXPECT_EQ(p2.y, 0);
}
