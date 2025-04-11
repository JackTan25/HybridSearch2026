

module;

export module index_emvb;

import stl;

import index_base;
import base_table_ref;
import create_index_info;
import third_party;
import statement_common;

namespace hybridsearch {

export class IndexEMVB final : public IndexBase {
public:
    IndexEMVB(SharedPtr<String> index_name,
              SharedPtr<String> index_comment,
              const String &file_name,
              Vector<String> column_names,
              const u32 residual_pq_subspace_num,
              const u32 residual_pq_subspace_bits)
        : IndexBase(IndexType::kEMVB, std::move(index_name), index_comment, file_name, std::move(column_names)),
          residual_pq_subspace_num_(residual_pq_subspace_num), residual_pq_subspace_bits_(residual_pq_subspace_bits) {}

    static SharedPtr<IndexBase> Make(SharedPtr<String> index_name,
                                     SharedPtr<String> index_comment,
                                     const String &file_name,
                                     Vector<String> column_names,
                                     const Vector<InitParameter *> &index_param_list);

    ~IndexEMVB() override = default;

    String BuildOtherParamsString() const override;

    static void ValidateColumnDataType(const SharedPtr<BaseTableRef> &base_table_ref, const String &column_name);

    i32 GetSizeInBytes() const override;

    void WriteAdv(char *&ptr) const override;

    String ToString() const override;

    nlohmann::json Serialize() const override;

    const u32 residual_pq_subspace_num_ = 0;
    const u32 residual_pq_subspace_bits_ = 0;
};

} // namespace hybridsearch
