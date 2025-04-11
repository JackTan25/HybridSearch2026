

module;

import stl;

export module like;

namespace hybridsearch {

class Catalog;

export void RegisterLikeFunction(const UniquePtr<Catalog> &catalog_ptr);

export void RegisterNotLikeFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch
