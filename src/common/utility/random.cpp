

module;

#include <cassert>
#include <cstdlib>

module random;

import stl;
import third_party;
import logger;
import virtual_store;
import default_values;
import hybridsearch_context;
import status;

namespace hybridsearch {

namespace {
String available_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
}

String RandomString(SizeT len) {
    // LOG_WARN(fmt::format("Set seed: {}", seed));
    String ret(len, '\0');
    for (SizeT i = 0; i < len; i++) {
        SizeT rand_i = random() % available_chars.size();
        ret[i] = available_chars[rand_i];
    }
    return ret;
}

SharedPtr<String> DetermineRandomString(const String &parent_dir, const String &name) {
    assert(std::filesystem::path(parent_dir).is_absolute());
    String rand, temp, result;
    int cnt = 0;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        srand(std::time(nullptr));
    }

    bool use_persistence_manager = hybridsearchContext::instance().persistence_manager();
    bool created = false;
    do {
        rand = RandomString(DEFAULT_RANDOM_NAME_LEN);
        result = fmt::format("{}_{}", rand, name);
        temp = VirtualStore::ConcatenatePath(parent_dir, result);
        ++cnt;
        if (!use_persistence_manager) {
            Status status = VirtualStore::MakeDirectory(temp);
            if (status.ok()) {
                created = true;
            } else {
                created = false;
            }
        } else {
            created = true;
        }
    } while (!created);
    LOG_DEBUG(fmt::format("Created directory {} in {} times", temp, cnt));
    return MakeShared<String>(std::move(result));
}

} // namespace hybridsearch
