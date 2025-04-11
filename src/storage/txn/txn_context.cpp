

module;

#include <sstream>

module txn_context;

import stl;

namespace hybridsearch {

String TxnContext::ToString() {
    std::stringstream ss;
    ss << "Txn ID: " << txn_id_;
    if(text_ != nullptr) {
        ss << ", Text: " << *text_;
    }
    ss << ", Begin TS: " << begin_ts_ << ", Commit TS: " << commit_ts_ << ", State: " << TxnState2Str(state_) << "\n";
    for (const auto &operation : operations_) {
        ss << *operation << "\n";
    }
    return ss.str();
}

}