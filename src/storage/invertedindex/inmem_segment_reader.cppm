

module;

export module inmem_index_segment_reader;

import stl;

import segment_posting;
import index_segment_reader;
import index_defines;
import posting_writer;
import memory_indexer;
import internal_types;

namespace hybridsearch {
export class InMemIndexSegmentReader : public IndexSegmentReader {
public:
    InMemIndexSegmentReader(SegmentID segment_id, MemoryIndexer *memory_indexer);
    virtual ~InMemIndexSegmentReader() = default;

    bool GetSegmentPosting(const String &term, SegmentPosting &seg_posting, bool fetch_position = true) const override;

private:
    SharedPtr<MemoryIndexer::PostingTable> posting_table_;
    RowID base_row_id_{INVALID_ROWID};
};

} // namespace hybridsearch