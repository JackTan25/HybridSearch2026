

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import stl;
import global_resource_usage;
import hybridsearch_context;

using namespace hybridsearch;
class TrimTest : public BaseTest {};

TEST_F(TrimTest, test1) {
    std::string path = "/usr/src/storage/db";
    std::string trimed_path = hybridsearch::TrimPath(path);
    ASSERT_EQ(trimed_path, "src/storage/db");
}
