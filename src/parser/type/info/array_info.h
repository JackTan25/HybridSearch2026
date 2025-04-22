

#pragma once

#include <utility>

#include "type/data_type.h"
#include "type/type_info.h"

namespace hybridsearch {

class ArrayInfo : public TypeInfo {
public:
    static std::shared_ptr<ArrayInfo> Make(DataType elem_type) { return std::make_shared<ArrayInfo>(std::move(elem_type)); }

    explicit ArrayInfo(DataType elem_type) : TypeInfo(TypeInfoType::kArray), elem_type_(std::move(elem_type)) {}

    ~ArrayInfo() override = default;

    bool operator==(const TypeInfo &other) const override;

    // Array always costs 8 bytes. Real data is stored in heap memory.
    [[nodiscard]] size_t Size() const override;

    [[nodiscard]] size_t ElemSize() const { return elem_type_.Size(); }

    [[nodiscard]] nlohmann::json Serialize() const override;

    [[nodiscard]] std::string ToString() const override;

    [[nodiscard]] const DataType &ElemType() const { return elem_type_; }

private:
    DataType elem_type_;
};

} // namespace hybridsearch
