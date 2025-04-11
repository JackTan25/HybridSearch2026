

module;

module peer_task;

import stl;
import third_party;

namespace hybridsearch {

String ToString(PeerTaskType type) {
    switch (type) {
        case PeerTaskType::kRegister: {
            return "register";
        }
        case PeerTaskType::kHeartBeat: {
            return "heartbeat";
        }
        case PeerTaskType::kUnregister: {
            return "unregister";
        }
        case PeerTaskType::kInvalid: {
            return "invalid";
        }
        case PeerTaskType::kChangeRole: {
            return "change role";
        }
        case PeerTaskType::kLogSync: {
            return "log sync";
        }
        case PeerTaskType::kNewLeader: {
            return "new leader";
        }
        case PeerTaskType::kTerminate: {
            return "terminate";
        }
    }
}

String TerminatePeerTask::ToString() const { return fmt::format("{}", hybridsearch::ToString(type_)); }

String RegisterPeerTask::ToString() const {
    return fmt::format("{}@{}, {}:{}, {}", hybridsearch::ToString(type_), node_name_, node_ip_, node_port_, hybridsearch::ToString(node_role_));
}

String UnregisterPeerTask::ToString() const { return fmt::format("{}@{}", hybridsearch::ToString(type_), node_name_); }

String HeartBeatPeerTask::ToString() const { return fmt::format("{}@{}, {}", hybridsearch::ToString(type_), node_name_, txn_ts_); }

String SyncLogTask::ToString() const { return fmt::format("{}@{}, {}", hybridsearch::ToString(type_), node_name_, log_strings_.size()); }

String ChangeRoleTask::ToString() const { return fmt::format("{} to {}", hybridsearch::ToString(type_), role_name_); }

} // namespace hybridsearch
