
module;

export module threadutil;

import stl;

namespace hybridsearch {

// Encapsulates methods for thread access.
export class ThreadUtil {
public:
    static bool pin(Thread &thread, const u16 cpu_id);
};

} // namespace hybridsearch