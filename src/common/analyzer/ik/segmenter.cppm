module;

export module segmenter;

import analyze_context;

namespace hybridsearch {

export class Segmenter {
public:
    virtual void Analyze(AnalyzeContext *context) = 0;

    virtual void Reset() = 0;

    virtual ~Segmenter() = default;
};

} // namespace hybridsearch