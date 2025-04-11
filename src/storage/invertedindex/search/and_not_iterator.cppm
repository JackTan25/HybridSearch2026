

module;

export module and_not_iterator;

import stl;
import index_defines;
import multi_doc_iterator;
import doc_iterator;
import internal_types;

namespace hybridsearch {
export class AndNotIterator : public MultiDocIterator {
public:
    AndNotIterator(Vector<UniquePtr<DocIterator>> iterators);

    DocIteratorType GetType() const override { return DocIteratorType::kAndNotIterator; }

    String Name() const override { return "AndNotIterator"; }

    /* pure virtual methods implementation */
    bool Next(RowID doc_id) override;

    float Score() override;

    void UpdateScoreThreshold(float threshold) override;

    u32 MatchCount() const override;
};
} // namespace hybridsearch