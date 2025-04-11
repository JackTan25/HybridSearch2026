

module;

module index_segment_reader;

import stl;

import segment_posting;
import index_defines;

namespace hybridsearch {
IndexSegmentReader::IndexSegmentReader(SegmentID segment_id, ChunkID chunk_id) : segment_id_(segment_id), chunk_id_(chunk_id) {}
} // namespace hybridsearch
