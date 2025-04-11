

module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <ijma.h>
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

module japanese_analyzer;

namespace fs = std::filesystem;

namespace hybridsearch {
static const String KNOWLEDGE_PATH = "mecab/ipadic";

// Foreign Language is Japanese POS
static constexpr const char *JAPANESE_FL = "S-A";

// Special character is Japanese POS
static constexpr const char *JAPANESE_SC = "S-G";

class JMAKnowledge {
private:
    JMAKnowledge(const char *knowledge_path) {
        if (knowledge_path != nullptr) {
            knowledge_ = jma::JMA_Factory::instance()->createKnowledge();
            // load system dictioanry files
            knowledge_->setSystemDict(knowledge_path);

            if (knowledge_->loadDict() == 0) {
                String msg = "Failed to load JMA knowledge from path: ";
                msg.append(knowledge_path);
                throw std::logic_error(msg);
            }
        }
    }
    ~JMAKnowledge() { delete knowledge_; }

public:
    static JMAKnowledge &instance(const char *knowledge_path) {
        static JMAKnowledge instance(knowledge_path);
        return instance;
    }

    jma::Knowledge *knowledge_{nullptr};
};

JapaneseAnalyzer::JapaneseAnalyzer(const String &base_path) {
    cjk_ = true;
    jma::JMA_Factory *factory = jma::JMA_Factory::instance();
    analyzer_ = factory->createAnalyzer();
    fs::path root(base_path);
    fs::path knowledge_path(root / KNOWLEDGE_PATH);
    knowledge_path_ = knowledge_path.string();
    sentence_ = new jma::Sentence();
}

JapaneseAnalyzer::JapaneseAnalyzer(const JapaneseAnalyzer &other) : knowledge_(other.knowledge_) {
    cjk_ = true;
    jma::JMA_Factory *factory = jma::JMA_Factory::instance();
    analyzer_ = factory->createAnalyzer();
    analyzer_->setKnowledge(knowledge_);
    sentence_ = new jma::Sentence();
    fl_morp_ = analyzer_->getCodeFromStr(JAPANESE_FL);
    sc_morp_ = analyzer_->getCodeFromStr(JAPANESE_SC);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_POS_FORMAT_ALPHABET, 1);

    SetCaseSensitive(false);
    SetIndexMode(); // Index mode is set by default
}

JapaneseAnalyzer::~JapaneseAnalyzer() {
    delete analyzer_;
    delete sentence_;
}

Status JapaneseAnalyzer::Load() {
    try {
        knowledge_ = JMAKnowledge::instance(knowledge_path_.c_str()).knowledge_;
    } catch (std::logic_error) {
        return Status::InvalidAnalyzerFile("Failed to load JMA analyzer");
    }
    analyzer_->setKnowledge(knowledge_);
    fl_morp_ = analyzer_->getCodeFromStr(JAPANESE_FL);
    sc_morp_ = analyzer_->getCodeFromStr(JAPANESE_SC);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_POS_FORMAT_ALPHABET, 1);

    SetCaseSensitive(false);
    SetIndexMode(); // Index mode is set by default

    return Status::OK();
}

void JapaneseAnalyzer::SetIndexMode() {
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_NBEST, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_DECOMPOSE_USER_NOUN, 0);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_COMPOUND_MORPHOLOGY, 0);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_CONVERT_TO_HALF_WIDTH, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_CONVERT_TO_LOWER_CASE, 0);
}

void JapaneseAnalyzer::SetLabelMode() {
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_NBEST, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_COMPOUND_MORPHOLOGY, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_DECOMPOSE_USER_NOUN, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_CONVERT_TO_HALF_WIDTH, 1);
    analyzer_->setOption(jma::Analyzer::OPTION_TYPE_CONVERT_TO_LOWER_CASE, 0);
}

} // namespace hybridsearch