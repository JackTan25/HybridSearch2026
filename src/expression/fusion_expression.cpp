

module;

#include <sstream>

module fusion_expression;

import stl;
import expression_type;

import scalar_function;
import search_options;
import hybridsearch_exception;
import third_party;

namespace hybridsearch {

FusionExpression::FusionExpression(const String &method, SharedPtr<SearchOptions> options)
    : BaseExpression(ExpressionType::kFusion, Vector<SharedPtr<BaseExpression>>()), method_(method), options_(std::move(options)) {}

String FusionExpression::ToString() const {
    if (!alias_.empty()) {
        return alias_;
    }
    String expr_str = fmt::format("FUSION('{}', '{}')", method_, options_ ? options_->ToString() : "");
    return expr_str;
}

} // namespace hybridsearch
