

#pragma once

#include <map>
#include <memory>
#include <string>

namespace hybridsearch {

struct QueryNode;

enum class FulltextQueryOperatorOption {
    khybridsearchSyntax, // use parser's syntax
    kAnd,            // combine all terms with AND
    kOr,             // combine all terms with OR
};

/**
 * Conducting the whole scanning and parsing.
 */
class SearchDriver {
public:
    SearchDriver(const std::map<std::string, std::string> &field2analyzer,
                 const std::string &default_field,
                 const FulltextQueryOperatorOption operator_option = FulltextQueryOperatorOption::khybridsearchSyntax)
        : field2analyzer_{field2analyzer}, default_field_{SearchDriver::Unescape(default_field)}, operator_option_(operator_option) {}

    // used in PhysicalMatch
    [[nodiscard]] std::unique_ptr<QueryNode> ParseSingleWithFields(const std::string &fields_str, const std::string &query) const;

    // used in ParseSingleWithFields and unit_test
    [[nodiscard]] std::unique_ptr<QueryNode> ParseSingle(const std::string &query, const std::string *default_field_ptr = nullptr) const;

    // used in SearchParser in ParseSingle. Assumes field and text are both unescaped.
    [[nodiscard]] std::unique_ptr<QueryNode>
    AnalyzeAndBuildQueryNode(const std::string &field, const std::string &text, bool from_quoted, unsigned long slop = 0) const;

    [[nodiscard]] static std::string Unescape(const std::string &text);

    /**
     * parsing options
     */
    const std::map<std::string, std::string> &field2analyzer_;
    const std::string default_field_;
    const FulltextQueryOperatorOption operator_option_ = FulltextQueryOperatorOption::khybridsearchSyntax;
};

} // namespace hybridsearch
