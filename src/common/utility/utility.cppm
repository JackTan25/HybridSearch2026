

module;

import stl;

export module utility;

export namespace hybridsearch::Utility {

SizeT NextPowerOfTwo(SizeT input);
String FormatByteSize(u64 byte_size);
String FormatTimeInfo(u64 seconds);

} // namespace hybridsearch::Utility

export namespace hybridsearch {

enum class IdentifierValidationStatus {
    kOk,
    kEmpty,
    kExceedLimit,
    kInvalidName,
};

IdentifierValidationStatus IdentifierValidation(const String &identifier);

bool ParseIPPort(const String &str, String &ip, i64 &port);

String StringTransform(const String &source, const String &from, const String &to);

String CalcMD5(const char* input_str, SizeT length);

String CalcMD5(const String& filename);

} // namespace hybridsearch
