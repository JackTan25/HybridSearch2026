

module;

export module multi_doc_iterator;

import stl;

import index_defines;
import doc_iterator;
import term_doc_iterator;

namespace hybridsearch {
export class MultiDocIterator : public DocIterator {
public:
    explicit MultiDocIterator(Vector<UniquePtr<DocIterator>> &&children) : children_(std::move(children)) {}

    const Vector<UniquePtr<DocIterator>> &GetChildren() { return children_; }

    void PrintTree(std::ostream &os, const String &prefix, bool is_final) const override;

protected:
    Vector<UniquePtr<DocIterator>> children_;
};
} // namespace hybridsearch