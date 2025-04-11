

module;

export module knn_filter;

import stl;
import hnsw_common;
import roaring_bitmap;

import segment_entry;

namespace hybridsearch {

export template <typename LabelType>
class BitmaskFilter final : public FilterBase<LabelType> {
public:
    explicit BitmaskFilter(const Bitmask &bitmask) : bitmask_(bitmask) {}

    bool operator()(const LabelType &label) const final { return bitmask_.IsTrue(label); }

private:
    const Bitmask &bitmask_;
};

export class AppendFilter final : public FilterBase<SegmentOffset> {
public:
    AppendFilter(SegmentOffset max_segment_offset) : max_segment_offset_(max_segment_offset) {}

    bool operator()(const SegmentOffset &segment_offset) const final { return segment_offset < max_segment_offset_; }

private:
    const SegmentOffset max_segment_offset_;
};

export class DeleteFilter final : public FilterBase<SegmentOffset> {
public:
    explicit DeleteFilter(const SegmentEntry *segment, TxnTimeStamp query_ts, SegmentOffset max_segment_offset)
        : segment_(segment), query_ts_(query_ts), max_segment_offset_(max_segment_offset) {}

    bool operator()(const SegmentOffset &segment_offset) const final {
        bool check_append = max_segment_offset_ == 0;
        return segment_offset <= max_segment_offset_ && segment_->CheckRowVisible(segment_offset, query_ts_, check_append);
    }

private:
    const SegmentEntry *const segment_;

    const TxnTimeStamp query_ts_;

    const SegmentOffset max_segment_offset_;
};

} // namespace hybridsearch
