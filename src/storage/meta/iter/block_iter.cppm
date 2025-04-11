

module;

#include <utility>
#include <vector>

export module block_iter;

import block_column_iter;
import stl;
import block_entry;
import block_column_iter;
import buffer_manager;
import hybridsearch_exception;
import logger;
import column_vector;

namespace hybridsearch {

export template <bool CheckTS = true>
class BlockIter {
public:
    BlockIter(const BlockEntry *entry, BufferManager *buffer_mgr, const Vector<ColumnID> &column_ids, TxnTimeStamp iterate_ts) {
        SizeT row_count = entry->row_count();
        for (auto column_id : column_ids) {
            column_iters_.emplace_back(entry->GetColumnBlockEntry(column_id), buffer_mgr, iterate_ts, row_count);
        }
    }

    Optional<Pair<Vector<const void *>, BlockOffset>> Next() {
        Vector<const void *> rets;
        BlockOffset offset = 0;
        for (auto &column_iter : column_iters_) {
            auto ret_opt = column_iter.Next();
            if (!ret_opt.has_value()) {
                if (rets.empty()) {
                    return None;
                }
                String error_message = "ColumnIter return None, but other columns have data";
                UnrecoverableError(error_message);
            }
            auto [ret, cur_offset] = *ret_opt;
            if (offset != cur_offset) {
                if (rets.empty()) {
                    offset = cur_offset;
                } else {
                    String error_message = "ColumnIter return different offset";
                    UnrecoverableError(error_message);
                }
            }
            rets.emplace_back(ret);
        }
        return std::make_pair(std::move(rets), offset);
    }

    const SharedPtr<ColumnVector> &column_vector(SizeT col_id) const { return column_iters_[col_id].column_vector(); }

    BlockOffset offset() const {
        BlockOffset offset = BlockOffset(-1);
        for (auto &column_iter : column_iters_) {
            if (offset == BlockOffset(-1)) {
                offset = column_iter.offset();
            } else if (offset != column_iter.offset()) {
                String error_message = "ColumnIter return different offset";
                UnrecoverableError(error_message);
            }
        }
        return offset;
    }

private:
    Vector<BlockColumnIter<CheckTS>> column_iters_;
};

} // namespace hybridsearch