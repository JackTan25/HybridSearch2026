

module;

export module japanese_analyzer;

import stl;
import ijma;
import term;
import common_analyzer;
import status;

namespace hybridsearch {

export class JapaneseAnalyzer : public CommonLanguageAnalyzer {
public:
    JapaneseAnalyzer(const String &path);

    JapaneseAnalyzer(const JapaneseAnalyzer &other);

    ~JapaneseAnalyzer();

    Status Load();

    void SetIndexMode();

    void SetLabelMode();

    void SetNBest(u32 num = 2) { analyzer_->setOption(jma::Analyzer::OPTION_TYPE_NBEST, num); }

protected:
    void Parse(const String &input) override {
        sentence_->setString(input.c_str());
        analyzer_->runWithSentence(*sentence_);

        list_index_ = 0;
        lexicon_index_ = -1;
        local_offset_ = -1;

        ResetToken();
    }

    bool NextToken() override {
        while (DoNext()) {
            token_ = sentence_->getLexicon(list_index_, lexicon_index_);
            len_ = std::strlen(token_);

            morpheme_ = sentence_->getPOS(list_index_, lexicon_index_);
            is_index_ = true; // sentence_->isIndexWord(list_index_, lexicon_index_);
            offset_ = local_offset_;
            return true;
        }
        ResetToken();
        return false;
    }

    bool IsAlpha() override { return morpheme_ == fl_morp_; }

    bool IsSpecialChar() override { return morpheme_ == sc_morp_; }

private:
    bool DoNext() {
        if (list_index_ == sentence_->getListSize()) {
            return false;
        }
        ++lexicon_index_;

        if (lexicon_index_ == sentence_->getCount(list_index_)) {
            do {
                ++list_index_;
                lexicon_index_ = 0;
                local_offset_ = 0;
                if (list_index_ == sentence_->getListSize())
                    return false;
            } while (lexicon_index_ == sentence_->getCount(list_index_));
        } else {
            ++local_offset_;
        }

        return true;
    }

private:
    String knowledge_path_;

    jma::Knowledge *knowledge_{nullptr};

    jma::Analyzer *analyzer_{nullptr};

    jma::Sentence *sentence_{nullptr};

    i32 list_index_;

    i32 lexicon_index_;

    u32 morpheme_;

    u32 fl_morp_;

    u32 sc_morp_;

    String segment_;
};
} // namespace hybridsearch
