

#include "parser/type/complex/varchar.h"
#include "gtest/gtest.h"
import base_test;

using namespace hybridsearch;
class VarcharTest : public BaseTest {};

TEST_F(VarcharTest, structure_check) {
    using namespace hybridsearch;
    EXPECT_EQ(sizeof(InlineVarchar), 13u);
    EXPECT_EQ(sizeof(VectorVarchar), 13u);
    EXPECT_EQ(sizeof(Varchar), 16u);
}
