module;

export module trunc;

import stl;

namespace hybridsearch {

class Catalog;
export void RegisterTruncFunction(const UniquePtr<Catalog> &catalog_ptr);

}