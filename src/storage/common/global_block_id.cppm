

module;

import stl;

export module global_block_id;

namespace hybridsearch {

export struct GlobalBlockID {
    SegmentID segment_id_;
    BlockID block_id_;

    GlobalBlockID(SegmentID segment_id, BlockID block_id) : segment_id_(segment_id), block_id_(block_id) {}

    bool operator==(const GlobalBlockID &other) const { return segment_id_ == other.segment_id_ && block_id_ == other.block_id_; }
};

export class GlobalBlockIDHash {
public:
    SizeT operator()(const GlobalBlockID &global_block_id) const {
        return Hash<SegmentID>()(global_block_id.segment_id_) ^ Hash<BlockID>()(global_block_id.block_id_);
    }
};

} // namespace hybridsearch
