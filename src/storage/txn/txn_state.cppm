

module;

export module txn_state;

import stl;
import logger;
import hybridsearch_exception;

namespace hybridsearch {

export enum class TxnState {
    kNotStarted,
    kStarted,
    kCommitting,
    kCommitted,
    kRollbacking,
    kRollbacked,
    kInvalid,
};

export inline String TxnState2Str(TxnState txn_state) {
    switch (txn_state) {
        case TxnState::kNotStarted: {
            return "Not Started";
        }
        case TxnState::kStarted: {
            return "Started";
        }
        case TxnState::kCommitting: {
            return "Committing";
        }
        case TxnState::kCommitted: {
            return "Committed";
        }
        case TxnState::kRollbacking: {
            return "Rollbacking";
        }
        case TxnState::kRollbacked: {
            return "Rollbacked";
        }
        default: {
            break;
        }
    }
    String error_message = "Invalid transaction state.";
    UnrecoverableError(error_message);
    return String();
}

export enum class TransactionType {
    kCheckpoint, // Develop know it's a checkpoint txn
    kRead,       // Developer know it's a read txn
    kNormal      // Developer doesn't know what type is this txn
};

export inline String TransactionType2Str(TransactionType txn_type) {
    switch (txn_type) {
        case TransactionType::kCheckpoint: {
            return "Checkpoint";
        }
        case TransactionType::kRead: {
            return "Read";
        }
        case TransactionType::kNormal: {
            return "Normal";
        }
    }
    return "Normal";
}

} // namespace hybridsearch
