

module;

export module current_timestamp;

import stl;

namespace hybridsearch {

class Catalog;

export void RegisterCurrentTimestampFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch