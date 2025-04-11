

module;

export module cross_product_table_ref;

import stl;

import table_ref;
import table_reference;

namespace hybridsearch {

export class CrossProductTableRef : public TableRef {
public:
    explicit CrossProductTableRef(String alias) : TableRef(TableRefType::kCrossProduct, alias) {}

    //    SharedPtr<BindContext> left_bind_context_{};
    //    SharedPtr<BindContext> right_bind_context_{};

    SharedPtr<TableRef> left_table_ref_{};
    SharedPtr<TableRef> right_table_ref_{};
};

} // namespace hybridsearch