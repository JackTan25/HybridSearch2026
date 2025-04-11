

module;

export module index_secondary;

import stl;

import index_base;
import base_table_ref;
import create_index_info;

namespace hybridsearch {

// Does not need any extra member.
export class IndexSecondary final : public IndexBase {
public:
    static SharedPtr<IndexBase>
    Make(SharedPtr<String> index_name, SharedPtr<String> index_comment, const String &file_name, Vector<String> column_names) {
        return MakeShared<IndexSecondary>(index_name, index_comment, file_name, std::move(column_names));
    }

    IndexSecondary(SharedPtr<String> index_name, SharedPtr<String> index_comment, const String &file_name, Vector<String> column_names)
        : IndexBase(IndexType::kSecondary, index_name, index_comment, file_name, std::move(column_names)) {}

    ~IndexSecondary() final = default;

    virtual String BuildOtherParamsString() const override;

    static void ValidateColumnDataType(const SharedPtr<BaseTableRef> &base_table_ref, const String &column_name);
};

} // namespace hybridsearch
