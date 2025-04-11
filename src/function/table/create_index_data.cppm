

module;

export module create_index_data;

import stl;
import segment_entry;
import block_index;
import third_party;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

export struct CreateIndexSharedData {
    CreateIndexSharedData() = default;

    explicit CreateIndexSharedData(BlockIndex *block_index) { Init(block_index); }

    void Init(BlockIndex *block_index) {
        for (const auto &[segment_id, segment_info] : block_index->segment_block_index_) {
            auto [iter, insert_ok] = create_index_idxes_.emplace(segment_id, 0);
            if (!insert_ok) {
                String error_message = fmt::format("Duplicate segment id: %u", segment_id);
                UnrecoverableError(error_message);
            }
        }
    }

    HashMap<u32, atomic_u64> create_index_idxes_{};
};

}; // namespace hybridsearch