

module;

import stl;

import segment_posting;
import index_defines;
export module index_segment_reader;

namespace hybridsearch {
export class IndexSegmentReader {
public:
    IndexSegmentReader(SegmentID segment_id, ChunkID chunk_id);
    virtual ~IndexSegmentReader() {}

    // fetch_position is only valid in DiskIndexSegmentReader
    virtual bool GetSegmentPosting(const String &term, SegmentPosting &seg_posting, bool fetch_position = true) const = 0;

    SegmentID segment_id() const { return segment_id_; }

    ChunkID chunk_id() const { return chunk_id_; }

private:
    SegmentID segment_id_;
    ChunkID chunk_id_;
};

} // namespace hybridsearch