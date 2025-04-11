// Copyright(C) 2024 InfiniFlow, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

module;

#include "internal_types.h"

export module internal_types;

namespace hybridsearch {

export using hybridsearch::BooleanT;

export using hybridsearch::TinyIntT;
export using hybridsearch::SmallIntT;
export using hybridsearch::IntegerT;
export using hybridsearch::BigIntT;
export using hybridsearch::HugeIntT;

export using hybridsearch::DoubleT;
export using hybridsearch::FloatT;

export using hybridsearch::DecimalT;

export using hybridsearch::VarcharT;
export using hybridsearch::DateT;

export using hybridsearch::TimeT;
export using hybridsearch::DateTimeT;
export using hybridsearch::DateTOld;
export using hybridsearch::TimestampT;
export using hybridsearch::IntervalT;

// Nest types
export using hybridsearch::ArrayT;
export using hybridsearch::TupleT;

// Geography
export using hybridsearch::PointT;
export using hybridsearch::LineT;
export using hybridsearch::LineSegT;
export using hybridsearch::BoxT;
export using hybridsearch::CircleT;

// Other
export using hybridsearch::UuidT;
export using hybridsearch::EmbeddingT;

// Heterogeneous
export using hybridsearch::MixedT;

// tensor
export using hybridsearch::TensorT;
// tensor-array
export using hybridsearch::TensorArrayT;

// sparse
export using hybridsearch::SparseT;

export using hybridsearch::EmptyArrayT;

// FP16
export using hybridsearch::Float16T;
// BF16
export using hybridsearch::BFloat16T;

// MultiVector
export using hybridsearch::MultiVectorT;

export using hybridsearch::RowID;
export using hybridsearch::TimeUnit;

export using hybridsearch::IntegerMixedType;
export using hybridsearch::FloatMixedType;
export using hybridsearch::ArrayMixedType;
export using hybridsearch::BaseMixedType;
export using hybridsearch::LongStrMixedType;
export using hybridsearch::MissingMixedType;
export using hybridsearch::ShortStrMixedType;
export using hybridsearch::MixedType;
export using hybridsearch::MixedValueType;
export using hybridsearch::TupleMixedType;

export using hybridsearch::EmbeddingType;
export using hybridsearch::ToEmbeddingDataType;
export using hybridsearch::EmbeddingDataType;

export constexpr uint64_t VARCHAR_PREFIX_LEN = VARCHAR_PREFIX_LENGTH;
export constexpr uint64_t VARCHAR_INLINE_LEN = VARCHAR_INLINE_LENGTH;
export constexpr uint64_t VARCHAR_LEN_LIMIT = VARCHAR_LENGTH_LIMIT;

} // namespace hybridsearch
