module;

module;

module inmem_index_segment_reader;

import stl;

import segment_posting;
import index_segment_reader;
import index_defines;
import posting_writer;
import memory_indexer;
import third_party;

namespace hybridsearch {
InMemIndexSegmentReader::InMemIndexSegmentReader(SegmentID segment_id, MemoryIndexer *memory_indexer)
    : IndexSegmentReader(segment_id, std::numeric_limits<ChunkID>::max()), posting_table_(memory_indexer->GetPostingTable()),
      base_row_id_(memory_indexer->GetBaseRowId()) {}

bool InMemIndexSegmentReader::GetSegmentPosting(const String &term, SegmentPosting &seg_posting, bool fetch_position) const {
    SharedPtr<PostingWriter> writer;
    bool found = posting_table_->store_.Get(term, writer);
    if (found) {
        seg_posting.Init(base_row_id_, writer);
        return true;
    }
    return false;
}

} // namespace hybridsearch