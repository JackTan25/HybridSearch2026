

#include "gtest/gtest.h"
import base_test;

import stl;
import var_buffer;
import hybridsearch_exception;

using namespace hybridsearch;

class VarBufferTest : public BaseTest {};

TEST_F(VarBufferTest, test1) {
    VarBuffer var_buffer;
    auto data = MakeUnique<char[]>(26);
    for (int i = 0; i < 26; ++i) {
        data[i] = 'a' + i;
    }
    char *empty_data = nullptr;

    SizeT offset1 = var_buffer.Append(data.get(), 26);
    EXPECT_EQ(offset1, 0);

    SizeT offset1_1 = var_buffer.Append(empty_data, 0);
    EXPECT_EQ(offset1_1, 26);

    SizeT offset2 = var_buffer.Append(data.get(), 26);
    EXPECT_EQ(offset2, 26);

    auto test = [&](const VarBuffer &var_buffer) {
        const auto *res1 = var_buffer.Get(0, 26);
        EXPECT_EQ(std::string_view(res1, 26), std::string_view(data.get(), 26));

        const auto *res1_1 = var_buffer.Get(26, 0);
        EXPECT_EQ(nullptr, res1_1);

        const auto *res2 = var_buffer.Get(26, 26);
        EXPECT_EQ(std::string_view(res2, 26), std::string_view(data.get(), 26));

        try {
            [[maybe_unused]] const auto *res3 = var_buffer.Get(52, 26);
            FAIL();
        } catch (UnrecoverableException &e) {
        }
    };

    test(var_buffer);
    auto size = var_buffer.TotalSize();
    auto buffer = MakeUnique<char[]>(size);
    auto *p = buffer.get();
    p += var_buffer.Write(p);
    EXPECT_EQ(p, buffer.get() + size);

    VarBuffer var_buffer2;
    var_buffer2.Append(buffer.get(), size);
    test(var_buffer2);
}
