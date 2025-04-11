

module;

export module meta_state;

import stl;

namespace hybridsearch {

struct SegmentEntry;
struct BlockEntry;
struct BlockColumnEntry;
class DataBlock;
class ColumnVector;

export struct MetaColumnVectorState {
    ColumnVector *column_vector_{};
};

export struct MetaLocalDataState {
    DataBlock *data_block_{};
    HashMap<u64, MetaColumnVectorState> column_vector_map_{};
};

export struct MetaBlockColumnState {
    BlockColumnEntry *block_column_{};
};

export struct MetaBlockState {
    BlockEntry *block_entry_{};
    HashMap<u64, MetaBlockColumnState> column_data_map_{};
};

export struct MetaSegmentState {
    SegmentEntry *segment_entry_{};

    HashMap<u16, MetaBlockState> block_map_{};
};

export struct MetaTableState {
    Vector<MetaLocalDataState> local_blocks_{};

    // segment id->meta segment state
    HashMap<u32, MetaSegmentState> segment_map_{};
};

} // namespace hybridsearch
