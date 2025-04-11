

module;

#include <vector>

export module hnsw_util;

import stl;
import statement_common;
import hybridsearch_exception;
import status;

namespace hybridsearch {

export struct HnswOptimizeOptions {
    bool compress_to_lvq = false;
    bool lvq_avg = false;
};

export struct HnswUtil {
    static Optional<HnswOptimizeOptions> ParseOptimizeOptions(const Vector<UniquePtr<InitParameter>> &opt_params) {
        HnswOptimizeOptions options;
        for (const auto &param : opt_params) {
            if (IsEqual(param->param_name_, "compress_to_lvq")) {
                options.compress_to_lvq = true;
            } else if (IsEqual(param->param_name_, "lvq_avg")) {
                options.lvq_avg = true;
            }
        }
        if (options.compress_to_lvq && options.lvq_avg) {
            RecoverableError(Status::InvalidIndexParam("compress_to_lvq and lvq_avg cannot be set at the same time"));
        }
        if (!options.compress_to_lvq && !options.lvq_avg) {
            return None;
        }
        return options;
    }
};

} // namespace hybridsearch