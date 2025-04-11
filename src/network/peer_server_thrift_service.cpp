

module;

module peer_server_thrift_service;

import stl;
import third_party;
import logger;
import peer_server_thrift_types;
import hybridsearch_context;
import peer_task;
import status;
import hybridsearch_exception;
import cluster_manager;
import admin_statement;
import node_info;

namespace hybridsearch {

void PeerServerThriftService::Register(hybridsearch_peer_server::RegisterResponse &response, const hybridsearch_peer_server::RegisterRequest &request) {

    LOG_TRACE("Get Register request");
    // This must be a leader node.
    NodeRole this_server_role = hybridsearchContext::instance().GetServerRole();
    if (this_server_role == NodeRole::kLeader) {
        NodeRole register_node_role = NodeRole::kUnInitialized;
        switch (request.node_type) {
            case hybridsearch_peer_server::NodeType::kFollower: {
                register_node_role = NodeRole::kFollower;
                break;
            }
            case hybridsearch_peer_server::NodeType::kLearner: {
                register_node_role = NodeRole::kLearner;
                break;
            }
            default: {
                String error_message = fmt::format("Invalid node type: {}", hybridsearch_peer_server::to_string(request.node_type));
                UnrecoverableError(error_message);
            }
        }
        auto now = std::chrono::system_clock::now();
        auto time_since_epoch = now.time_since_epoch();

        SharedPtr<NodeInfo> register_node_info = MakeShared<NodeInfo>(register_node_role,
                                                                      NodeStatus::kAlive,
                                                                      request.node_name,
                                                                      request.node_ip,
                                                                      request.node_port,
                                                                      request.txn_timestamp,
                                                                      std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch).count());
        Status status = hybridsearchContext::instance().cluster_manager()->AddNodeInfo(register_node_info);
        if (status.ok()) {
            LOG_INFO(fmt::format("Node: {} registered as {}.", request.node_name, hybridsearch_peer_server::to_string(request.node_type)));
            SharedPtr<NodeInfo> leader_node = hybridsearchContext::instance().cluster_manager()->ThisNode();
            response.leader_name = leader_node->node_name();
            response.leader_term = leader_node->leader_term();
            response.heart_beat_interval = leader_node->heartbeat_interval();
        } else {
            LOG_ERROR(fmt::format("Node: {} fail to register with leader, error: {}", request.node_name, status.message()));
            response.error_code = static_cast<i64>(status.code());
            response.error_message = status.message();
        }
    } else {
        response.error_code = static_cast<i64>(ErrorCode::kInvalidNodeRole);
        response.error_message = "Attempt to register with a non-leader node";
    }

    return;
}

void PeerServerThriftService::Unregister(hybridsearch_peer_server::UnregisterResponse &response, const hybridsearch_peer_server::UnregisterRequest &request) {
    LOG_TRACE("Get Unregister request");
    NodeRole this_server_role = hybridsearchContext::instance().GetServerRole();
    if (this_server_role == NodeRole::kLeader) {
        Status status = hybridsearchContext::instance().cluster_manager()->UpdateNodeByLeader(request.node_name, UpdateNodeOp::kRemove);
        if (!status.ok()) {
            response.error_code = static_cast<i64>(status.code_);
            response.error_message = status.message();
        }
        LOG_INFO(fmt::format("Node: {} unregistered from leader.", request.node_name));
    } else {
        response.error_code = static_cast<i64>(ErrorCode::kInvalidNodeRole);
        response.error_message = "Attempt to unregister from a non-leader node";
    }
    return;
}

void PeerServerThriftService::HeartBeat(hybridsearch_peer_server::HeartBeatResponse &response, const hybridsearch_peer_server::HeartBeatRequest &request) {
    LOG_DEBUG("Get HeartBeat request");
    NodeRole this_server_role = hybridsearchContext::instance().GetServerRole();
    if (this_server_role == NodeRole::kLeader) {
        NodeRole non_leader_node_role = NodeRole::kUnInitialized;
        switch (request.node_type) {
            case hybridsearch_peer_server::NodeType::kLeader: {
                non_leader_node_role = NodeRole::kLeader;
                break;
            }
            case hybridsearch_peer_server::NodeType::kFollower: {
                non_leader_node_role = NodeRole::kFollower;
                break;
            }
            case hybridsearch_peer_server::NodeType::kLearner: {
                non_leader_node_role = NodeRole::kLearner;
                break;
            }
            default: {
                String error_message = "Invalid node type";
                UnrecoverableError(error_message);
            }
        }

        auto now = std::chrono::system_clock::now();
        auto time_since_epoch = now.time_since_epoch();
        SharedPtr<NodeInfo> register_node_info = MakeShared<NodeInfo>(non_leader_node_role,
                                                                      NodeStatus::kAlive,
                                                                      request.node_name,
                                                                      request.node_ip,
                                                                      request.node_port,
                                                                      request.txn_timestamp,
                                                                      std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch).count());

        Status status = hybridsearchContext::instance().cluster_manager()->UpdateNodeInfoByHeartBeat(register_node_info,
                                                                                                 response.other_nodes,
                                                                                                 response.leader_term,
                                                                                                 response.sender_status);
        if (!status.ok()) {
            response.error_code = static_cast<i64>(status.code());
            response.error_message = status.message();
        }
    } else {
        response.error_code = static_cast<i64>(ErrorCode::kInvalidNodeRole);
        response.sender_status = hybridsearch_peer_server::NodeStatus::type::kRemoved;
        response.error_message = fmt::format("Attempt to heartbeat from a non-leader node: {}", ToString(this_server_role));
    }
    return;
}

void PeerServerThriftService::SyncLog(hybridsearch_peer_server::SyncLogResponse &response, const hybridsearch_peer_server::SyncLogRequest &request) {
    LOG_INFO("Get SyncLog request");
    if (request.log_entries.size() == 0) {
        UnrecoverableError("No log is synced from leader node");
    }

    hybridsearchContext::instance().storage()->wal_manager()->FlushLogByReplication(request.log_entries, request.on_startup);

    Status status = Status::OK();
    if (request.on_startup) {
        status = hybridsearchContext::instance().cluster_manager()->ContinueStartup(request.log_entries);
    } else {
        status = hybridsearchContext::instance().cluster_manager()->ApplySyncedLogNolock(request.log_entries);
    }

    if (!status.ok()) {
        response.error_code = static_cast<i64>(status.code());
        response.error_message = status.message();
    }
    return;
}

void PeerServerThriftService::ChangeRole(hybridsearch_peer_server::ChangeRoleResponse &response, const hybridsearch_peer_server::ChangeRoleRequest &request) {
    Status status = Status::OK();
    switch (request.node_type) {
        case hybridsearch_peer_server::NodeType::kAdmin: {
            status = hybridsearchContext::instance().ChangeServerRole(NodeRole::kAdmin, true);
            break;
        }
        default: {
            UnrecoverableError("Not support to change to other type of node");
        }
    }

    if (!status.ok()) {
        response.error_code = static_cast<i64>(status.code());
        response.error_message = status.message();
    }
    return;
}

void PeerServerThriftService::NewLeader(hybridsearch_peer_server::NewLeaderResponse &response, const hybridsearch_peer_server::NewLeaderRequest &request) {
    LOG_INFO("Get NewLeader request");
    return;
}

} // namespace hybridsearch
