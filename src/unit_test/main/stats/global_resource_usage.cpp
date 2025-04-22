

#include "gtest/gtest.h"
import base_test;

import stl;
import global_resource_usage;

using namespace hybridsearch;
class GlobalResourceUsageTest : public BaseTest {
    void SetUp() override {
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
    }

    void TearDown() override {
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
    }
};

TEST_F(GlobalResourceUsageTest, usage_test) {
    using namespace hybridsearch;

    // Object count
#ifdef hybridsearch_STATS
    GlobalResourceUsage::IncrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), 1);

    GlobalResourceUsage::IncrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), 2);

    GlobalResourceUsage::DecrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), 1);

    GlobalResourceUsage::DecrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), 0);

    GlobalResourceUsage::DecrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), -1);

    GlobalResourceUsage::IncrObjectCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetObjectCount(), 0);

    // Raw memory count
    GlobalResourceUsage::IncrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), 1);

    GlobalResourceUsage::IncrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), 2);

    GlobalResourceUsage::DecrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), 1);

    GlobalResourceUsage::DecrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), 0);

    GlobalResourceUsage::DecrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), -1);

    GlobalResourceUsage::IncrRawMemCount("GlobalResourceUsageTest");
    EXPECT_EQ(GlobalResourceUsage::GetRawMemoryCount(), 0);
#endif
}