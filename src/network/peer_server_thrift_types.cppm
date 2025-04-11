// Copyright(C) 2024 InfiniFlow, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

module;

#include "peer_server_thrift/PeerService.h"

export module peer_server_thrift_types;

namespace hybridsearch_peer_server {

export using hybridsearch_peer_server::PeerServiceIf;
export using hybridsearch_peer_server::PeerServiceClient;
export using hybridsearch_peer_server::PeerServiceIfFactory;
export using hybridsearch_peer_server::PeerServiceProcessorFactory;
export using hybridsearch_peer_server::NodeStatus;
export using hybridsearch_peer_server::NodeType;
export using hybridsearch_peer_server::NodeInfo;
export using hybridsearch_peer_server::RegisterRequest;
export using hybridsearch_peer_server::RegisterResponse;
export using hybridsearch_peer_server::UnregisterRequest;
export using hybridsearch_peer_server::UnregisterResponse;
export using hybridsearch_peer_server::HeartBeatRequest;
export using hybridsearch_peer_server::HeartBeatResponse;
export using hybridsearch_peer_server::SyncLogRequest;
export using hybridsearch_peer_server::SyncLogResponse;
export using hybridsearch_peer_server::ChangeRoleRequest;
export using hybridsearch_peer_server::ChangeRoleResponse;
export using hybridsearch_peer_server::NewLeaderRequest;
export using hybridsearch_peer_server::NewLeaderResponse;
export using hybridsearch_peer_server::to_string;
} // namespace hybridsearch_peer_server
