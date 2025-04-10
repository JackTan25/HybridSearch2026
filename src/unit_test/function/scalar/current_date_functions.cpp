

#include "gtest/gtest.h"

import stl;
import base_test;
import hybridsearch_exception;
import hybridsearch_context;

import catalog;
import logger;

import default_values;
import value;

import base_expression;
import column_expression;
import column_vector;
import data_block;

import function_set;
import function;

import global_resource_usage;

import data_type;
import internal_types;
import logical_type;

import scalar_function;
import scalar_function_set;

import current_date;
import third_party;

using namespace hybridsearch;

class CurrentDateFunctionsTest : public BaseTestParamStr {};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams, CurrentDateFunctionsTest, ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH));

TEST_P(CurrentDateFunctionsTest, current_date_func) {
    using namespace hybridsearch;

    UniquePtr<Catalog> catalog_ptr = MakeUnique<Catalog>();

    RegisterCurrentDateFunction(catalog_ptr);

    String op = "current_date";

    SharedPtr<FunctionSet> function_set = Catalog::GetFunctionSetByName(catalog_ptr.get(), op);
    EXPECT_EQ(function_set->type_, FunctionType::kScalar);
    SharedPtr<ScalarFunctionSet> scalar_function_set = std::static_pointer_cast<ScalarFunctionSet>(function_set);

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        DataType data_type1(LogicalType::kVarchar);
        SharedPtr<DataType> result_type = MakeShared<DataType>(LogicalType::kDate);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("current_date()", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(MakeShared<DataType>(data_type1));

        DataBlock data_block;

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(result_type);
        result->Initialize();
        func.function_(data_block, result);

        for (SizeT i = 0; i < 1; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kDate);
        }
    }
}
