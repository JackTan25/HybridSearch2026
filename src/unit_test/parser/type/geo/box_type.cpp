

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
class BoxTypeTest : public BaseTest {};

TEST_F(BoxTypeTest, point1) {
    using namespace hybridsearch;

    BoxT b1;
    b1.upper_left.x = 1.0f;
    b1.upper_left.y = 2.0f;
    b1.lower_right.x = 3.0f;
    b1.lower_right.y = 4.0f;

    BoxT b2(b1);

    EXPECT_EQ(b2.upper_left.x, 1.0f);
    EXPECT_EQ(b2.upper_left.y, 2.0f);
    EXPECT_EQ(b2.lower_right.x, 3.0f);
    EXPECT_EQ(b2.lower_right.y, 4.0f);

    EXPECT_EQ(b1, b2);

    b1.Reset();

    EXPECT_EQ(b1.upper_left.x, 0);
    EXPECT_EQ(b1.upper_left.y, 0);
    EXPECT_EQ(b1.lower_right.x, 0);
    EXPECT_EQ(b1.lower_right.x, 0);
}
