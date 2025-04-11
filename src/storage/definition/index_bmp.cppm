

module;

export module index_bmp;

import stl;
import index_base;
import statement_common;
import third_party;
import base_table_ref;
import create_index_info;
import bmp_util;

namespace hybridsearch {

export class IndexBMP final : public IndexBase {
public:
    static SharedPtr<IndexBMP> Make(SharedPtr<String> index_name,
                                    SharedPtr<String> index_comment,
                                    const String &file_name,
                                    Vector<String> column_names,
                                    const Vector<InitParameter *> &index_param_list);

    static void ValidateColumnDataType(const SharedPtr<BaseTableRef> &base_table_ref, const String &column_name);

    IndexBMP(SharedPtr<String> index_name,
             SharedPtr<String> index_comment,
             const String &file_name,
             Vector<String> column_names,
             SizeT block_size,
             BMPCompressType compress_type)
        : IndexBase(IndexType::kBMP, std::move(index_name), index_comment, file_name, column_names), block_size_(block_size),
          compress_type_(compress_type) {}

public:
    virtual i32 GetSizeInBytes() const override;

    virtual void WriteAdv(char *&ptr) const override;

    virtual String ToString() const override;

    virtual String BuildOtherParamsString() const override;

    virtual nlohmann::json Serialize() const override;

public:
    SizeT block_size_{};
    BMPCompressType compress_type_{BMPCompressType::kCompressed};
};

} // namespace hybridsearch
