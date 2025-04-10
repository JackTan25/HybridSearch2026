

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import logger;
import heap_chunk;
import third_party;
import stl;
import global_resource_usage;
import hybridsearch_context;
import default_values;

using namespace hybridsearch;

class StringChunkTest : public BaseTest {};

TEST_F(StringChunkTest, string_chunk_a) {
    using namespace hybridsearch;

    StringHeapMgr string_chunk_mgr;

    EXPECT_THROW(string_chunk_mgr.Allocate(0), UnrecoverableException);

    for (u64 i = 256; i <= 8192; i += 256) {
        string_chunk_mgr.Allocate(i);
    }

    EXPECT_EQ(string_chunk_mgr.current_chunk_idx(), 26u);
    EXPECT_EQ(string_chunk_mgr.current_chunk_size(), 8192u);
}
