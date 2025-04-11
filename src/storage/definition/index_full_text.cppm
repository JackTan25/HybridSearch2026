

module;

export module index_full_text;

import stl;
import index_base;

import third_party;
import index_base;
import create_index_info;
import statement_common;
import base_table_ref;
import index_defines;

namespace hybridsearch {

export class IndexFullText final : public IndexBase {
public:
    static SharedPtr<IndexBase> Make(SharedPtr<String> index_name,
                                     SharedPtr<String> index_comment,
                                     const String &file_name,
                                     Vector<String> column_names,
                                     const Vector<InitParameter *> &index_param_list);

    IndexFullText(SharedPtr<String> index_name,
                  SharedPtr<String> index_comment,
                  const String &file_name,
                  Vector<String> column_names,
                  const String &analyzer,
                  optionflag_t flag = OPTION_FLAG_ALL)
        : IndexBase(IndexType::kFullText, index_name, index_comment, file_name, std::move(column_names)), analyzer_(analyzer), flag_(flag) {};

    ~IndexFullText() final = default;

    bool operator==(const IndexFullText &other) const;

    bool operator!=(const IndexFullText &other) const;

public:
    virtual i32 GetSizeInBytes() const override;

    virtual void WriteAdv(char *&ptr) const override;

    static SharedPtr<IndexBase> ReadAdv(char *&ptr, i32 maxbytes);

    virtual String ToString() const override;

    virtual String BuildOtherParamsString() const override;

    virtual nlohmann::json Serialize() const override;

    static SharedPtr<IndexFullText> Deserialize(const nlohmann::json &index_def_json);

    bool IsRealtime() const { return FlagIsRealtime(flag_); }

public:
    static void ValidateColumnDataType(const SharedPtr<BaseTableRef> &base_table_ref, const String &column_name);

public:
    String analyzer_{};
    optionflag_t flag_{OPTION_FLAG_ALL};
};

} // namespace hybridsearch
