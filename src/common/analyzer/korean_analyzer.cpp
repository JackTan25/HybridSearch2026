

module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <mecab_wrapper.h>
#pragma clang diagnostic pop

#include <cstring>
#include <filesystem>
#include <iostream>
import stl;
import term;
import analyzer;
import common_analyzer;
import logger;
import status;
import ijma;

module korean_analyzer;

namespace fs = std::filesystem;

namespace hybridsearch {
static const String KNOWLEDGE_PATH = "mecab/ko-dic";

KoreanAnalyzer::KoreanAnalyzer(const String &base_path) {
    cjk_ = true;
    own_mecab_ = true;
    fs::path root(base_path);
    fs::path knowledge_path(root / KNOWLEDGE_PATH);
    knowledge_path_ = "-d " + knowledge_path.string();
}

KoreanAnalyzer::KoreanAnalyzer(const KoreanAnalyzer &other) {
    cjk_ = true;
    knowledge_path_ = other.knowledge_path_;
    own_mecab_ = false;
    SetCaseSensitive(false);
}

KoreanAnalyzer::~KoreanAnalyzer() {
    if (own_mecab_)
        delete mecab_;
}

Status KoreanAnalyzer::Load() {
    try {
        mecab_ = new jma::MeCab(knowledge_path_);
    } catch (std::logic_error) {
        return Status::InvalidAnalyzerFile("Failed to load Korean analyzer");
    }

    SetCaseSensitive(false);

    return Status::OK();
}

} // namespace hybridsearch