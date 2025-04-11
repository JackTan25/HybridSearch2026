

#include "json.hpp"

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;
import decimal_info;
import parser_assert;

using namespace hybridsearch;
class DecimalInfoTest : public BaseTest {};

TEST_F(DecimalInfoTest, decimal_info_A) {
    using namespace hybridsearch;

    EXPECT_THROW(DecimalInfo::Make(39, 39), ParserException);
    EXPECT_THROW(DecimalInfo::Make(38, 39), ParserException);

    auto decimal_info = DecimalInfo::Make(38, 38);
    EXPECT_EQ(decimal_info->scale(), 38);
    EXPECT_EQ(decimal_info->precision(), 38);
    EXPECT_EQ(decimal_info->Size(), 16u);

    nlohmann::json json;
    json["type_info"] = decimal_info->Serialize();
    EXPECT_EQ(json.dump(), "{\"type_info\":{\"precision\":38,\"scale\":38}}");
}
