

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import stl;
import global_resource_usage;
import third_party;
import logger;
import compilation_config;
import hybridsearch_context;

using namespace hybridsearch;

class JsonTest : public BaseTest {
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(JsonTest, test1) {
    using namespace hybridsearch;

    String json_path = String(test_data_path()) + "/json/twitter.json";
    std::ifstream f(json_path);

    nlohmann::json data = nlohmann::json::parse(f);
    EXPECT_EQ(data["search_metadata"]["count"], 100);
}
