

module;

export module obj_status;

import stl;
import third_party;

namespace hybridsearch {

class Status;

export struct Range {
    SizeT start_{}; // inclusive
    SizeT end_{};   // exclusive
    bool operator<(const Range &rhs) const { return start_ < rhs.start_; }
    bool operator==(const Range &rhs) const { return start_ == rhs.start_ && end_ == rhs.end_; }
    bool Cover(const Range &rhs) const { return start_ <= rhs.start_ && rhs.end_ <= end_; }
    bool Intersect(const Range &rhs) const { return start_ < rhs.end_ && rhs.start_ < end_; }
};

export enum class ObjCached {
    kNotCached,
    kDownloading,
    kCached,
};

export struct ObjStat {
    SizeT obj_size_{};  // footer (if present) is excluded
    SizeT parts_{};     // an object attribute
    SizeT ref_count_{}; // the number of user (R and W) of some part of this object
    Set<Range> deleted_ranges_{};

    Atomic<ObjCached> cached_ = ObjCached::kCached; // whether the object is in localdisk cache

    ObjStat() = default;

    ObjStat(SizeT obj_size, SizeT parts, SizeT ref_count, ObjCached cached = ObjCached::kCached)
        : obj_size_(obj_size), parts_(parts), ref_count_(ref_count), cached_(cached) {}

    ObjStat(const ObjStat &other)
        : obj_size_(other.obj_size_), parts_(other.parts_), ref_count_(other.ref_count_), deleted_ranges_(other.deleted_ranges_),
          cached_(other.cached_.load()) {}

    ObjStat &operator=(const ObjStat &other) {
        if (this != &other) {
            obj_size_ = other.obj_size_;
            parts_ = other.parts_;
            ref_count_ = other.ref_count_;
            deleted_ranges_ = other.deleted_ranges_;
            cached_.store(other.cached_.load());
        }
        return *this;
    }

    ObjStat(ObjStat &&other)
        : obj_size_(other.obj_size_), parts_(other.parts_), ref_count_(other.ref_count_), deleted_ranges_(std::move(other.deleted_ranges_)),
          cached_(other.cached_.load()) {}

    ObjStat &operator=(ObjStat &&other) {
        if (this != &other) {
            obj_size_ = other.obj_size_;
            parts_ = other.parts_;
            ref_count_ = other.ref_count_;
            deleted_ranges_ = std::move(other.deleted_ranges_);
            cached_.store(other.cached_.load());
        }
        return *this;
    }

    nlohmann::json Serialize() const;

    void Deserialize(const nlohmann::json &obj);

    SizeT GetSizeInBytes() const;

    void WriteBufAdv(char *&buf) const;

    static ObjStat ReadBufAdv(const char *&buf);

    void CheckValid(const String &obj_key, SizeT current_object_size) const;
};

} // namespace hybridsearch