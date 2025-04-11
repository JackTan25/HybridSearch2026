

module;

export module logical_import;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import internal_types;
import statement_common;
import data_type;
import meta_info;

namespace hybridsearch {

export class LogicalImport : public LogicalNode {
public:
    explicit inline LogicalImport(u64 node_id,
                                  const SharedPtr<TableInfo> &table_info,
                                  String file_path,
                                  bool header,
                                  char delimiter,
                                  CopyFileType type)
        : LogicalNode(node_id, LogicalNodeType::kImport), table_info_(table_info), file_type_(type), file_path_(std::move(file_path)),
          header_(header), delimiter_(delimiter) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalImport"; }

    [[nodiscard]] inline const SharedPtr<TableInfo> table_info() const { return table_info_; }

    [[nodiscard]] inline CopyFileType FileType() const { return file_type_; }

    [[nodiscard]] inline const String &file_path() const { return file_path_; }

    [[nodiscard]] bool header() const { return header_; }

    [[nodiscard]] char delimiter() const { return delimiter_; }

private:
    SharedPtr<TableInfo> table_info_{};
    CopyFileType file_type_{CopyFileType::kInvalid};
    String file_path_{};
    bool header_{false};
    char delimiter_{','};
};

} // namespace hybridsearch
