

#include "gtest/gtest.h"
import base_test;

import stl;
import binary_fuse_filter;

using namespace hybridsearch;
class BinaryFuseFilterTest : public BaseTest {};

TEST_F(BinaryFuseFilterTest, test_2000) {
    using namespace hybridsearch;
    constexpr u64 NUM = 2000;
    std::array<u64, NUM> data;
    for (u64 i = 0; i < NUM; ++i) {
        data[i] = i * NUM;
    }
    BinaryFuse filter;
    // test build
    EXPECT_NO_THROW(filter.Build(0, data.data(), NUM));
    // test contain
    for (u64 i = 0; i < NUM; ++i) {
        u64 item = i * NUM;
        EXPECT_EQ(filter.Contain(1, item), true);
    }
    // test fake contain
    u64 fake_contain = 0;
    u64 total_cnt = 0;
    for (u64 i = 0; i < NUM * NUM; ++i) {
        if (i % NUM) {
            ++total_cnt;
            if (filter.Contain(1, i)) {
                ++fake_contain;
            }
        }
    }
    f64 ratio = static_cast<f64>(fake_contain) / total_cnt;
    EXPECT_LT(ratio, 0.005);
}
