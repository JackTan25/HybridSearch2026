

export module database_detail;
import stl;

namespace hybridsearch {

export struct DatabaseDetail {
    SharedPtr<String> db_name_{};
    SharedPtr<String> db_entry_dir_{};
    SharedPtr<String> db_comment_{};
};

} // namespace hybridsearch
