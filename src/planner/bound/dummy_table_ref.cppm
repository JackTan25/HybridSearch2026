

module;

import stl;

import table_ref;
import table_reference;

export module dummy_table_ref;

namespace hybridsearch {

class DummyTableRef : public TableRef {
public:
    explicit DummyTableRef() : TableRef(TableRefType::kDummy, "Dummy") {}
};

} // namespace hybridsearch
