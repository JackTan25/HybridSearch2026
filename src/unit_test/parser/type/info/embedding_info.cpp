

#include "json.hpp"
#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;
import internal_types;
import logger;
import stl;
import hybridsearch_context;
import embedding_info;
import knn_expr;
import parser_assert;
import parser_defaults;

using namespace hybridsearch;
class EmbeddingInfoTest : public BaseTest {};

TEST_F(EmbeddingInfoTest, embedding_info_A) {
    using namespace hybridsearch;

    EXPECT_THROW(EmbeddingInfo::Make(EmbeddingDataType::kElemFloat, EMBEDDING_LIMIT + 1), ParserException);

    auto embedding_info_ptr = EmbeddingInfo::Make(EmbeddingDataType::kElemBit, 256);
    EXPECT_EQ(embedding_info_ptr->Size(), 32u);
    EXPECT_EQ(embedding_info_ptr->Dimension(), 256u);
    EXPECT_EQ(embedding_info_ptr->Type(), EmbeddingDataType::kElemBit);

    embedding_info_ptr = EmbeddingInfo::Make(EmbeddingDataType::kElemInt64, EMBEDDING_LIMIT);
    EXPECT_EQ(embedding_info_ptr->Size(), SizeT(EMBEDDING_LIMIT * 8));
    EXPECT_EQ(embedding_info_ptr->Dimension(), (u64)EMBEDDING_LIMIT);
    EXPECT_EQ(embedding_info_ptr->Type(), EmbeddingDataType::kElemInt64);

    nlohmann::json json;
    json["type_info"] = embedding_info_ptr->Serialize();
    //    std::cout << json.dump() << std::endl;
    //    EXPECT_EQ(json.dump(), "{\"type_info\":{\"precision\":38,\"scale\":38}}");
}
