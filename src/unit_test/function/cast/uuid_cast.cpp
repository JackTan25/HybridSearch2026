

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;

import function_set;
import aggregate_function_set;
import aggregate_function;
import function;
import column_expression;
import value;
import default_values;
import data_block;
import uuid_cast;
import column_vector;
import bound_cast_func;
import internal_types;
import logical_type;
#if 0
using namespace hybridsearch;
class UuidCastTest : public BaseTest {};

TEST_F(UuidCastTest, uuid_cast0) {
    using namespace hybridsearch;

    // Try to cast uuid type to wrong type.
    {
        char uuid_str[17] = "aabbccddeeffgghh";
        UuidT source;
        source.Set(uuid_str);

        TinyIntT target;
        EXPECT_THROW(UuidTryCastToVarlen::Run(source, target, nullptr), UnrecoverableException);
    }
    {
        char uuid_str[17] = "aabbccddeeffgghh";
        UuidT source;
        source.Set(uuid_str);
        VarcharT target;

        SharedPtr<DataType> data_type = MakeShared<DataType>(LogicalType::kVarchar);
        SharedPtr<ColumnVector> col_varchar_ptr = MakeShared<ColumnVector>(data_type);
        col_varchar_ptr->Initialize();

        EXPECT_TRUE(UuidTryCastToVarlen::Run(source, target, col_varchar_ptr.get()));

        target.Reset(false);
    }
}

TEST_F(UuidCastTest, uuid_cast1) {
    using namespace hybridsearch;

    // Call BindUuidCast with wrong type of parameters
    {
        DataType target_type(LogicalType::kDecimal);
        EXPECT_THROW(BindUuidCast(target_type), UnrecoverableException);
    }

    SharedPtr<DataType> source_type = MakeShared<DataType>(LogicalType::kUuid);
    SharedPtr<ColumnVector> col_source = MakeShared<ColumnVector>(source_type);
    col_source->Initialize();
    for (i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++i) {
        String s('a' + i % 26, 16);
        UuidT uuid(s.c_str());

        Value v = Value::MakeUuid(uuid);
        col_source->AppendValue(v);
    }
    for (i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++i) {
        String s('a' + i % 26, 16);
        UuidT uuid(s.c_str());
        Value vx = col_source->GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kUuid);
        EXPECT_EQ(vx.value_.uuid, uuid);
    }
    // cast uuid column vector to varchar column vector
    {
        SharedPtr<DataType> target_type = MakeShared<DataType>(LogicalType::kVarchar);
        auto source2target_ptr = BindUuidCast(*target_type);
        EXPECT_NE(source2target_ptr.function, nullptr);

        SharedPtr<ColumnVector> col_target = MakeShared<ColumnVector>(target_type);
        col_target->Initialize();

        CastParameters cast_parameters;
        EXPECT_TRUE(source2target_ptr.function(col_source, col_target, DEFAULT_VECTOR_SIZE, cast_parameters));

        for (i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++i) {
            String s('a' + i % 26, 16);
            UuidT uuid(s.c_str());
            String uuid_str(uuid.body, 16);

            Value vx = col_target->GetValue(i);
            const String &s2 = vx.GetString();
            EXPECT_EQ(s2, s);
        }
    }
}
#endif