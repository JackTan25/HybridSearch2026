

module;

export module special_function;

import stl;
import function;
import data_type;
import internal_types;

namespace hybridsearch {

export enum class SpecialType {
    kRowID,
    kDistance,
    kDistanceFactors,
    kSimilarity,
    kSimilarityFactors,
    kScore,
    kScoreFactors,
    kCreateTs,
    kDeleteTs,
    kFilterFullText,
};

export class SpecialFunction final : public Function {
public:
    SpecialFunction(String func_name, DataType data_type, const SizeT extra_idx, const SpecialType special_type)
        : Function(std::move(func_name), FunctionType::kSpecial), data_type_(std::move(data_type)), special_type_(special_type),
          extra_idx_(extra_idx) {}

    const DataType &data_type() const { return data_type_; }

    SizeT extra_idx() const { return extra_idx_; }

    SpecialType special_type() const { return special_type_; }

    String ToString() const override { return name(); }

private:
    DataType data_type_;
    SpecialType special_type_;
    SizeT extra_idx_{0};
};

} // namespace hybridsearch
