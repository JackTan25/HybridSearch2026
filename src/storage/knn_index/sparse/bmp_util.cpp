

module;

#include <string>
#include <vector>

module bmp_util;

import logger;
import third_party;

namespace hybridsearch {

BmpSearchOptions BMPUtil::ParseBmpSearchOptions(const Vector<UniquePtr<InitParameter>> &opt_params) {
    BmpSearchOptions options;
    for (const auto &opt_param : opt_params) {
        if (opt_param->param_name_ == "alpha") {
            f32 alpha = std::stof(opt_param->param_value_);
            if (alpha < 0.0 || alpha > 1.0) {
                LOG_WARN("Invalid alpha value, should be in [0, 1]");
                continue;
            }
            options.alpha_ = alpha;
        } else if (opt_param->param_name_ == "beta") {
            f32 beta = std::stof(opt_param->param_value_);
            if (beta < 0.0 || beta > 1.0) {
                LOG_WARN("Invalid beta value, should be in [0, 1]");
                continue;
            }
            options.beta_ = beta;
        } else if (opt_param->param_name_ == "use_tail") {
            if (!IsEqual(opt_param->param_value_, "T") && !IsEqual(opt_param->param_value_, "F")) {
                LOG_WARN("Invalid use_tail value, should be T or F");
                continue;
            }
            options.use_tail_ = IsEqual(opt_param->param_value_, "T");
        } else if (opt_param->param_name_ == "use_lock") {
            if (!IsEqual(opt_param->param_value_, "T") && !IsEqual(opt_param->param_value_, "F")) {
                LOG_WARN("Invalid use_lock value, should be T or F");
                continue;
            }
            options.use_lock_ = IsEqual(opt_param->param_value_, "T");
        }
    }
    return options;
};

Optional<BMPOptimizeOptions> BMPUtil::ParseBMPOptimizeOptions(const Vector<UniquePtr<InitParameter>> &opt_params) {
    BMPOptimizeOptions options;
    for (const auto &opt_param : opt_params) {
        if (opt_param->param_name_ == "topk") {
            i32 topk = std::stoi(opt_param->param_value_);
            if (topk <= 0) {
                continue;
            }
            if (topk > 1000) {
                LOG_WARN(fmt::format("topk value is large {}", topk));
            }
            options.topk_ = topk;
        } else if (opt_param->param_name_ == "bp_reorder") {
            options.bp_reorder_ = true;
        }
    }
    if (options.topk_ == 0 && !options.bp_reorder_) {
        return None;
    }
    return options;
}

} // namespace hybridsearch