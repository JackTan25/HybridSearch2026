

module;

#include <__iterator/next.h>

module obj_status;

import serialize;
import status;
import logger;
import third_party;

namespace hybridsearch {

nlohmann::json ObjStat::Serialize() const {
    nlohmann::json obj;
    obj["obj_size"] = obj_size_;
    obj["parts"] = parts_;
    obj["deleted_ranges"] = nlohmann::json::array();
    for (auto &range : deleted_ranges_) {
        nlohmann::json range_obj;
        range_obj["start"] = range.start_;
        range_obj["end"] = range.end_;
        obj["deleted_ranges"].emplace_back(range_obj);
    }
    return obj;
}

void ObjStat::Deserialize(const nlohmann::json &obj) {
    ref_count_ = 0;
    obj_size_ = obj["obj_size"];
    parts_ = obj["parts"];
    if (obj.contains("deleted_ranges")) {
        SizeT start = 0;
        SizeT end = 0;
        for (auto &range_obj : obj["deleted_ranges"]) {
            if (range_obj.contains("start")) {
                start = range_obj["start"];
            }
            if (range_obj.contains("end")) {
                end = range_obj["end"];
            }
            deleted_ranges_.emplace(Range{.start_ = start, .end_ = end});
        }
    }
}

SizeT ObjStat::GetSizeInBytes() const {
    SizeT size = sizeof(SizeT) + sizeof(SizeT) + sizeof(SizeT);
    size += (sizeof(SizeT) + sizeof(SizeT)) * deleted_ranges_.size();
    return size;
}

void ObjStat::WriteBufAdv(char *&buf) const {
    ::hybridsearch::WriteBufAdv(buf, obj_size_);
    ::hybridsearch::WriteBufAdv(buf, parts_);
    ::hybridsearch::WriteBufAdv(buf, deleted_ranges_.size());
    for (auto &range : deleted_ranges_) {
        ::hybridsearch::WriteBufAdv(buf, range.start_);
        ::hybridsearch::WriteBufAdv(buf, range.end_);
    }
}

ObjStat ObjStat::ReadBufAdv(const char *&buf) {
    ObjStat ret;
    ret.obj_size_ = ::hybridsearch::ReadBufAdv<SizeT>(buf);
    ret.parts_ = ::hybridsearch::ReadBufAdv<SizeT>(buf);
    ret.ref_count_ = 0;

    SizeT start, end;
    SizeT len = ::hybridsearch::ReadBufAdv<SizeT>(buf);
    for (SizeT i = 0; i < len; ++i) {
        start = ::hybridsearch::ReadBufAdv<SizeT>(buf);
        end = ::hybridsearch::ReadBufAdv<SizeT>(buf);
        ret.deleted_ranges_.emplace(Range{.start_ = start, .end_ = end});
    }
    return ret;
}

void ObjStat::CheckValid(const String &obj_key, SizeT current_object_size) const {
    const Set<Range> &deleted_ranges = deleted_ranges_;
    if (deleted_ranges.size() >= 2) {
        auto it1 = deleted_ranges.begin();
        auto it2 = std::next(it1);
        while (it2 != deleted_ranges.end()) {
            if (it1->end_ >= it2->start_) {
                String error_message = fmt::format("CurrentObjFinalize Object {} deleted ranges intersect: [{}, {}), [{}, {})",
                                                   obj_key,
                                                   it1->start_,
                                                   it1->end_,
                                                   it2->start_,
                                                   it2->end_);
                LOG_ERROR(error_message);
            }
            it1 = it2;
            it2 = std::next(it2);
        }
    } else if (deleted_ranges.size() == 1) {
        auto it1 = deleted_ranges.begin();
        if (it1->start_ == 0 && it1->end_ == current_object_size) {
            String error_message = fmt::format("CurrentObjFinalize Object {} is fully deleted", obj_key);
            LOG_ERROR(error_message);
        }
    }
}

} // namespace hybridsearch
