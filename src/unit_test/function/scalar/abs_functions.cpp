

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;
import catalog;
import abs;
import scalar_function;
import scalar_function_set;
import function_set;
import function;
import column_expression;
import value;
import default_values;
import data_block;
import base_expression;
import column_vector;
import logical_type;
import internal_types;
import data_type;

using namespace hybridsearch;
class AbsFunctionsTest : public BaseTestParamStr {};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams, AbsFunctionsTest, ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH));

TEST_P(AbsFunctionsTest, abs_func) {
    using namespace hybridsearch;

    UniquePtr<Catalog> catalog_ptr = MakeUnique<Catalog>();

    RegisterAbsFunction(catalog_ptr);

    String op = "abs";
    SharedPtr<FunctionSet> function_set = Catalog::GetFunctionSetByName(catalog_ptr.get(), op);
    EXPECT_EQ(function_set->type_, FunctionType::kScalar);
    SharedPtr<ScalarFunctionSet> scalar_function_set = std::static_pointer_cast<ScalarFunctionSet>(function_set);

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kTinyInt);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(TinyInt)->TinyInt", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        SizeT row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (SizeT i = 0; i < row_count; ++i) {
            data_block.AppendValue(0, Value::MakeTinyInt(static_cast<i8>(i)));
        }
        data_block.Finalize();

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kTinyInt);
            EXPECT_EQ(v.value_.tiny_int, static_cast<i8>(i));
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kTinyInt);
            i8 res = static_cast<i8>(i);
            if (res == std::numeric_limits<i8>::min()) {
                EXPECT_EQ(result->nulls_ptr_->IsTrue(i), false);
            } else {
                EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
                EXPECT_EQ(v.value_.tiny_int, abs(res));
            }
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kSmallInt);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(SmallInt)->SmallInt", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        SizeT row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (SizeT i = 0; i < row_count; ++i) {
            data_block.AppendValue(0, Value::MakeSmallInt(static_cast<i16>(-i)));
        }
        data_block.Finalize();

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kSmallInt);
            EXPECT_EQ(v.value_.small_int, -static_cast<i16>(i));
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kSmallInt);
            i16 res = static_cast<i16>(i);
            EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
            EXPECT_EQ(v.value_.small_int, abs(res));
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kInteger);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(Integer)->Integer", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        SizeT row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (SizeT i = 0; i < row_count; ++i) {
            data_block.AppendValue(0, Value::MakeInt(static_cast<i32>(-i)));
        }
        data_block.Finalize();

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kInteger);
            EXPECT_EQ(v.value_.integer, -static_cast<i32>(i));
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kInteger);
            i32 res = static_cast<i32>(i);
            EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
            EXPECT_EQ(v.value_.integer, abs(res));
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kBigInt);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(BigInt)->BigInt", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        SizeT row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (SizeT i = 0; i < row_count; ++i) {
            data_block.AppendValue(0, Value::MakeBigInt(static_cast<i64>(-i)));
        }
        data_block.Finalize();

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kBigInt);
            EXPECT_EQ(v.value_.big_int, -static_cast<i64>(i));
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (SizeT i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kBigInt);
            i64 res = static_cast<i64>(i);
            EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
            EXPECT_EQ(v.value_.big_int, abs(res));
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kFloat);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(Float)->Float", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        i64 row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (i64 i = 0; i < row_count; ++i) {
            f32 input = i - 16384;
            input -= 0.5;
            data_block.AppendValue(0, Value::MakeFloat(input));
        }
        data_block.Finalize();

        for (i64 i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kFloat);

            f32 input = i - 16384;
            input -= 0.5;
            EXPECT_FLOAT_EQ(v.value_.float32, input);
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (i64 i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kFloat);
            f32 input = i - 16384;
            input -= 0.5;
            EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
            EXPECT_FLOAT_EQ(v.value_.float32, std::fabs(input));
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kDouble);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(*data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);
        EXPECT_STREQ("ABS(Double)->Double", func.ToString().c_str());

        Vector<SharedPtr<DataType>> column_types;
        column_types.emplace_back(data_type);

        i64 row_count = DEFAULT_VECTOR_SIZE;

        DataBlock data_block;
        data_block.Init(column_types);

        for (i64 i = 0; i < row_count; ++i) {
            f64 input = i - 16384;
            input -= 0.5;
            data_block.AppendValue(0, Value::MakeDouble(input));
        }
        data_block.Finalize();

        for (i64 i = 0; i < row_count; ++i) {
            Value v = data_block.GetValue(0, i);
            EXPECT_EQ(v.type_.type(), LogicalType::kDouble);

            f64 input = i - 16384;
            input -= 0.5;
            EXPECT_FLOAT_EQ(v.value_.float64, input);
        }

        SharedPtr<ColumnVector> result = MakeShared<ColumnVector>(data_type);
        result->Initialize();
        func.function_(data_block, result);

        for (i64 i = 0; i < row_count; ++i) {
            Value v = result->GetValue(i);
            EXPECT_EQ(v.type_.type(), LogicalType::kDouble);
            f64 input = i - 16384;
            input -= 0.5;
            EXPECT_EQ(result->nulls_ptr_->IsTrue(i), true);
            EXPECT_FLOAT_EQ(v.value_.float64, std::fabs(input));
        }
    }

    {
        Vector<SharedPtr<BaseExpression>> inputs;

        DataType data_type(LogicalType::kHugeInt);
        SharedPtr<ColumnExpression> col_expr_ptr = MakeShared<ColumnExpression>(data_type, "t1", 1, "c1", 0, 0);

        inputs.emplace_back(col_expr_ptr);
        ScalarFunction func = scalar_function_set->GetMostMatchFunction(inputs);

        EXPECT_STREQ("ABS(Float)->Float", func.ToString().c_str());
    }
}
