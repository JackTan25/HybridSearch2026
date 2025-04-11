

module;

export module blockmax_leaf_iterator;

import stl;
import internal_types;
import doc_iterator;
import column_length_io;

namespace hybridsearch {

export class BlockMaxLeafIterator : public DocIterator {
public:
    // ref: https://en.wikipedia.org/wiki/Okapi_BM25
    virtual void InitBM25Info(UniquePtr<FullTextColumnLengthReader> &&column_length_reader, float delta, float k1, float b) = 0;

    virtual RowID BlockMinPossibleDocID() const = 0;

    virtual RowID BlockLastDocID() const = 0;

    virtual float BlockMaxBM25Score() = 0;

    // Move block cursor to ensure its last_doc_id is no less than given doc_id.
    // Returns false and update doc_id_ to INVALID_ROWID if the iterator is exhausted.
    // Note that this routine decode skip_list only, and doesn't update doc_id_ when returns true.
    // Caller may invoke BlockMaxBM25Score() after this routine.
    virtual bool NextShallow(RowID doc_id) = 0;

    virtual float BM25Score() = 0;
};

} // namespace hybridsearch
