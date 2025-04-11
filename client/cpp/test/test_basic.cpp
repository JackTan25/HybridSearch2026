

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