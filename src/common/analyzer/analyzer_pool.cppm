

module;

export module analyzer_pool;

import stl;
import singleton;
import third_party;
import analyzer;
import status;

namespace hybridsearch {

export class AnalyzerPool : public Singleton<AnalyzerPool> {
public:
    using CacheType = FlatHashMap<std::string_view, UniquePtr<Analyzer>>;

    Tuple<UniquePtr<Analyzer>, Status> GetAnalyzer(const std::string_view &name);

    static u64 AnalyzerNameToInt(const char *str);

    void Set(const std::string_view &name);

public:
    static constexpr std::string_view CHINESE = "chinese";
    static constexpr std::string_view TRADITIONALCHINESE = "traditional";
    static constexpr std::string_view JAPANESE = "japanese";
    static constexpr std::string_view KOREAN = "korean";
    static constexpr std::string_view STANDARD = "standard";
    static constexpr std::string_view NGRAM = "ngram";
    static constexpr std::string_view RAG = "rag";
    static constexpr std::string_view IK = "ik";
    static constexpr std::string_view KEYWORD = "keyword";
    static constexpr std::string_view WHITESPACE = "whitespace";
    static constexpr std::string_view RANKFEATURES = "rankfeatures";

private:
    CacheType cache_{};
};

} // namespace hybridsearch