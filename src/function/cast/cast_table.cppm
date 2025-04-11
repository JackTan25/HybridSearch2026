

module;

export module cast_table;

import singleton;
import stl;

import logical_type;

namespace hybridsearch {

export class CastTable : public Singleton<CastTable> {
public:
    CastTable();

public:
    [[nodiscard]] i64 GetCastCost(LogicalType from, LogicalType to) const;

private:
    Array<Array<i64, to_underlying_val(LogicalType::kInvalid)>, to_underlying_val(LogicalType::kInvalid)> matrix_{};
};

} // namespace hybridsearch
