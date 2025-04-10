

module;

#include <iomanip>
#include <openssl/md5.h>
#include <regex>
#include <sstream>

module utility;

import stl;
import third_party;
import default_values;

namespace hybridsearch::Utility {

SizeT NextPowerOfTwo(SizeT input) {
    --input;
    input |= input >> 1;
    input |= input >> 2;
    input |= input >> 4;
    input |= input >> 8;
    input |= input >> 16;
    input |= input >> 32;
    return ++input;
}

String FormatByteSize(u64 byte_size) {
    static const char *sizeSuffixes[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

    if (byte_size == 0) {
        return "0B";
    }

    int suffixIndex = static_cast<int>(std::log2(byte_size) / 10);
    double size = static_cast<double>(byte_size) / (1 << (suffixIndex * 10));

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << sizeSuffixes[suffixIndex];
    return oss.str();
}

String FormatTimeInfo(u64 seconds) {
    u64 second = seconds % 60;
    seconds -= second;
    if (seconds == 0) {
        return fmt::format("{}s", second);
    }

    seconds /= 60;
    u64 minute = seconds % 60;
    seconds -= minute;
    if (seconds == 0) {
        return fmt::format("{}m", minute);
    }

    seconds /= 60;
    return fmt::format("{}h", seconds);
}

} // namespace hybridsearch::Utility

namespace hybridsearch {

IdentifierValidationStatus IdentifierValidation(const String &identifier) {
    if (identifier.empty()) {
        return IdentifierValidationStatus::kEmpty;
    }

    u64 identifier_len = identifier.length();
    if (identifier_len >= MAX_IDENTIFIER_NAME_LENGTH) {
        return IdentifierValidationStatus::kExceedLimit;
    }

    if (!std::isalpha(identifier[0]) && identifier[0] != '_') {
        return IdentifierValidationStatus::kInvalidName;
    }
    for (SizeT i = 1; i < identifier_len; i++) {
        char ch = identifier[i];
        if (!std::isalnum(ch) && ch != '_') {
            return IdentifierValidationStatus::kInvalidName;
        }
    }

    return IdentifierValidationStatus::kOk;
}

bool ParseIPPort(const String &str, String &ip, i64 &port) {
    static const std::regex pattern("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d{1,5})$");
    std::smatch matches;

    try {
        if (std::regex_match(str, matches, pattern)) {
            ip = matches[1];
            port = std::stoi(matches[2]);
            return true;
        }

        return false;
    } catch (std::exception &e) {
        return false;
    }
}

String StringTransform(const String &source, const String &from, const String &to) {
    String ret(source);
    size_t start_pos = 0;
    while ((start_pos = ret.find(from, start_pos)) != String::npos) {
        ret.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return ret;
}

String CalcMD5(const char *input_str, SizeT length) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input_str, length);

    unsigned char md[MD5_DIGEST_LENGTH];
    MD5_Final(md, &ctx);

    std::stringstream hex_stream;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        hex_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md[i]);
    }

    return hex_stream.str();
}

String CalcMD5(const String& input_file) {
    std::ifstream input(input_file, std::ios::binary);
    input.seekg(0, input.end);
    size_t raw_file_size = input.tellg();
    input.seekg(0, input.beg);
    std::vector<char> raw_data(raw_file_size);
    input.read(raw_data.data(), raw_file_size);
    input.close();
    String md5 = CalcMD5(raw_data.data(), raw_file_size);
    return md5;
}

} // namespace hybridsearch
