

#pragma once
#include <map>
#include <string>
#include <utility>
namespace hybridsearch {

class SearchOptions {
public:
    explicit SearchOptions(const std::string &options_str);
    bool operator==(const SearchOptions &other) const;
    bool operator!=(const SearchOptions &other) const { return !(*this == other); }

    std::string ToString();
    size_t size() const { return options_.size(); }
    std::map<std::string, std::string> options_;

private:
    static std::pair<std::string, std::string> ParseOption(const std::string_view &option);
};

} // namespace hybridsearch