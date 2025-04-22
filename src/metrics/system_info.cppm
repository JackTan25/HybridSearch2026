

module;

export module system_info;

import stl;

namespace hybridsearch {

export class SystemInfo {
public:
    static i64 MemoryUsage();
    static f64 CPUUsage();
    static i64 OpenFileCount();
};

} // namespace hybridsearch
