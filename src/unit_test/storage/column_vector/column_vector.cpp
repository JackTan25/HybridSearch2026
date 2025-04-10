

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import logger;
import column_vector;
import value;

import default_values;
import third_party;
import stl;
import global_resource_usage;
import hybridsearch_context;
import internal_types;
import logical_type;
import data_type;

using namespace hybridsearch;
class ColumnVectorTest : public BaseTest {};

TEST_F(ColumnVectorTest, ReadWrite) {
    using namespace hybridsearch;

    Vector<SharedPtr<ColumnVector>> columns;
    SizeT vector_size = DEFAULT_VECTOR_SIZE / 3;
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kBoolean));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeBool(bool(i % 2));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kTinyInt));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeTinyInt(i8(i));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kSmallInt));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeSmallInt(i16(i));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kInteger));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeInt(i32(i));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kBigInt));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeBigInt(i64(i));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
#if 0
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kHugeInt));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeHugeInt(HugeIntT(i, i));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
#endif
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kFloat));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeFloat(float(i) + 0.5f);
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kDouble));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeDouble(double(i) + 0.5f);
            column->AppendValue(v);
        }
        columns.push_back(column);
    }
    {
        SharedPtr<ColumnVector> column = ColumnVector::Make(MakeShared<DataType>(LogicalType::kVarchar));
        column->Initialize();
        for (SizeT i = 0; i < vector_size; ++i) {
            Value v = Value::MakeVarchar(String(i, 'a' + i % 26));
            column->AppendValue(v);
        }
        columns.push_back(column);
    }

    /*
    // TODO: Following case cause exception due to the value created by `Value::MakeBitmap` has null info_ptr.
         {
            SharedPtr<ColumnVector> column =
                ColumnVector::Make(MakeShared<DataType>(LogicalType::kBitmap,
       BitmapInfo::Make(1024))); column->Initialize(); for (SizeT i = 0; i <
       vector_size; ++i) { BitmapT bitmap; bitmap.Initialize(i + 10); for (i64 j
       = 0; j <= i; ++j) { if (j % 2 == 0) { bitmap.SetBit(j, true); } else {
                        bitmap.SetBit(j, false);
                    }
                }
                Value v = Value::MakeBitmap(bitmap);
                column->AppendValue(v);
            }
            columns.push_back(column);
        }
     */
    for (SizeT i = 0; i < columns.size(); i++) {
        int32_t exp_size = columns[i]->GetSizeInBytes();
        int32_t act_size;
        Vector<char> buf(exp_size);
        char *ptr = buf.data();

        columns[i]->WriteAdv(ptr);
        act_size = ptr - buf.data();
        EXPECT_EQ(act_size, exp_size);

        const char *ptr_r = buf.data();
        SharedPtr<ColumnVector> column2 = ColumnVector::ReadAdv(ptr_r, exp_size);
        act_size = ptr_r - buf.data();
        EXPECT_EQ(act_size, exp_size);
        EXPECT_NE(column2, nullptr);
        EXPECT_EQ(*columns[i] == *column2, true);
    }
}
