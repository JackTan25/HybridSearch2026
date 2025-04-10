

#pragma once

#include "json.hpp"
#include <cstdint>
#include <string>

namespace hybridsearch {

enum class TypeInfoType : uint8_t {
    kInvalid,

    // Primitive
    kDecimal,
    kVarchar,

    // Nested
    kArray,
    kTuple, // Dictionary

    // Geography
    kPoint,
    kLine,
    kLineSeg,
    kBox,
    kPath,
    kPolygon,
    kCircle,

    // Other
    kBitmap,
    kUUID,
    kBlob,
    kEmbedding,
    kSparse,
};

class TypeInfo {
public:
    explicit TypeInfo(TypeInfoType type) : type_(type) {};

    virtual ~TypeInfo() = default;

    virtual bool operator==(const TypeInfo &other) const = 0;

    bool operator!=(const TypeInfo &other) const;

    [[nodiscard]] virtual size_t Size() const = 0;

    [[nodiscard]] inline TypeInfoType type() const noexcept { return type_; }

    [[nodiscard]] virtual nlohmann::json Serialize() const = 0;

    [[nodiscard]] virtual std::string ToString() const = 0;

protected:
    TypeInfoType type_{TypeInfoType::kInvalid};
};

} // namespace hybridsearch
