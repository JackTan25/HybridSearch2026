

module;

#include <string>
module ivf_index_search;

import stl;
import index_ivf;
import internal_types;
import logical_type;
import data_type;
import knn_expr;
import knn_scan_data;
import hybridsearch_exception;
import status;
import third_party;
import ivf_index_data;
import ivf_index_data_in_mem;
import ivf_index_storage;

namespace hybridsearch {

IVF_Search_Params IVF_Search_Params::Make(const KnnScanFunctionData *knn_scan_function_data) {
    IVF_Search_Params params;
    params.knn_distance_ = knn_scan_function_data->knn_distance_.get();
    const auto *knn_scan_shared_data = knn_scan_function_data->knn_scan_shared_data_;
    params.knn_scan_shared_data_ = knn_scan_shared_data;
    if (knn_scan_shared_data->query_count_ != 1) {
        RecoverableError(Status::SyntaxError(fmt::format("Invalid query_count: {} which is not 1.", knn_scan_shared_data->query_count_)));
    }
    params.topk_ = knn_scan_shared_data->topk_;
    params.query_embedding_ = knn_scan_shared_data->query_embedding_;
    params.query_elem_type_ = knn_scan_shared_data->query_elem_type_;
    params.knn_distance_type_ = knn_scan_shared_data->knn_distance_type_;
    params.nprobe_ = 1;
    for (const auto &opt_param : knn_scan_shared_data->opt_params_) {
        if (opt_param.param_name_ == "nprobe") {
            params.nprobe_ = DataType::StringToValue<IntegerT>(opt_param.param_value_);
            if (params.nprobe_ <= 0) {
                RecoverableError(Status::SyntaxError(fmt::format("Invalid negative nprobe value: {}", opt_param.param_name_)));
            }
        }
    }
    if (params.topk_ <= 0 || params.topk_ > std::numeric_limits<u32>::max()) {
        RecoverableError(Status::SyntaxError(fmt::format("Invalid topk which is out of range: {}.", params.topk_)));
    }
    return params;
}

} // namespace hybridsearch
