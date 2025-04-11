

module;

export module txn_context;

import stl;
import txn_state;

namespace hybridsearch {

export struct TxnContext {
    // This struct is used to store the operation history of a transaction. This history can be used for debugging purposes.
    static UniquePtr<TxnContext> Make() { return MakeUnique<TxnContext>(); }

    void AddOperation(const SharedPtr<String> &operation_text) { operations_.push_back(operation_text); }
    Vector<SharedPtr<String>> GetOperations() const { return operations_; }
    String ToString();

    TransactionID txn_id_{};
    TxnTimeStamp begin_ts_{};
    TxnTimeStamp commit_ts_{};
    TxnState state_{TxnState::kStarted};
    TransactionType txn_type_{TransactionType::kRead};

    bool is_write_transaction_{false};

    SharedPtr<String> text_{};
    Vector<SharedPtr<String>> operations_;
};

} // namespace hybridsearch
