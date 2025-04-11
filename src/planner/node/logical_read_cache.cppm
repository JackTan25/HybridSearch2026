

module;

export module logical_read_cache;

import stl;
import logical_node;
import data_block;
import column_binding;
import data_type;
import base_table_ref;
import logical_node_type;

namespace hybridsearch {

struct CacheContent;

export class LogicalReadCache : public LogicalNode {
public:
    LogicalReadCache(u64 node_id,
                     LogicalNodeType origin_type,
                     SharedPtr<BaseTableRef> base_table_ref,
                     SharedPtr<CacheContent> cache_content,
                     Vector<SizeT> column_map,
                     bool is_min_heap);

public:
    virtual Vector<ColumnBinding> GetColumnBindings() const;

    virtual SharedPtr<Vector<String>> GetOutputNames() const;

    virtual SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const;

    virtual String ToString(i64 &space) const;

    virtual String name() { return "LogicalReadCache"; }

public:
    LogicalNodeType origin_type_;
    SharedPtr<BaseTableRef> base_table_ref_;
    SharedPtr<CacheContent> cache_content_;
    Vector<SizeT> column_map_; // result column id -> cache column id
    bool is_min_heap_;
};

} // namespace hybridsearch