

module;

#include <cstring>
#include <filesystem>

#include <openccxx.h>

import stl;
import logger;
import third_party;
import chinese_analyzer;

module traditional_chinese_analyzer;

namespace fs = std::filesystem;

namespace hybridsearch {
static const String OPENCC_PATH = "opencc";

TraditionalChineseAnalyzer::TraditionalChineseAnalyzer(const String &path) : ChineseAnalyzer(path) {}

TraditionalChineseAnalyzer::TraditionalChineseAnalyzer(const TraditionalChineseAnalyzer &other) : ChineseAnalyzer(other) { own_opencc_ = false; }

TraditionalChineseAnalyzer::~TraditionalChineseAnalyzer() {
    if (own_opencc_) {
        delete opencc_;
    }
}

Status TraditionalChineseAnalyzer::Load() {
    try {
        ChineseAnalyzer::Load();
    } catch (const std::exception &e) {
        return Status::InvalidAnalyzerFile("Failed to load jieba analyzer");
    }
    fs::path root(dict_path_);
    fs::path opencc_path(root / OPENCC_PATH);

    if (!fs::exists(opencc_path)) {
        return Status::InvalidAnalyzerFile(opencc_path);
    }
    try {
        opencc_ = new ::OpenCC(opencc_path.string());
    } catch (const std::exception &e) {
        return Status::InvalidAnalyzerFile("Failed to load OpenCC");
    }
    own_opencc_ = true;
    return Status::OK();
}

void TraditionalChineseAnalyzer::Parse(const String &input) {
    String out;
    opencc_->convert(input, out);
    ChineseAnalyzer::Parse(out);
}

} // namespace hybridsearch