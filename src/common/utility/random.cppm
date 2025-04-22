module;

import stl;

export module random;

namespace hybridsearch {

export String RandomString(SizeT len);

// Return randomized `name`
export SharedPtr<String> DetermineRandomString(const String &parent_dir, const String &name);

} // namespace hybridsearch