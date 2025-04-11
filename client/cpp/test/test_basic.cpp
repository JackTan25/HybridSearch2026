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

#include "hybridsearch_client.h"
#include "iostream"

int main() {
    hybridsearch::client::Client hybridsearch_client = hybridsearch::client::Client::Connect("127.0.0.1", 23817);

    hybridsearch::client::ClientStatus status = hybridsearch_client.CreateDatabase("db1");
    if(!status.IsOK()) {
        std::cerr << status.ErrorMessage() << std::endl;
        return static_cast<int>(status.ErrorCode());
    }

    std::vector<std::string> db_names = hybridsearch_client.ListDatabases();
    for(auto& db_name: db_names) {
        std::cout << db_name << std::endl;
    }

    status = hybridsearch_client.DropDatabase("db1");
    if(!status.IsOK()) {
        std::cerr << status.ErrorMessage() << std::endl;
        return static_cast<int>(status.ErrorCode());
    }

    db_names = hybridsearch_client.ListDatabases();
    for(auto& db_name: db_names) {
        std::cout << db_name << std::endl;
    }

    hybridsearch_client.Disconnect();
    return 0;
}