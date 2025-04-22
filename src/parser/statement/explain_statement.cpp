

#include "explain_statement.h"

namespace hybridsearch {

ExplainStatement::~ExplainStatement() {
    if (statement_ != nullptr) {
        delete statement_;
        statement_ = nullptr;
    }
}

std::string ExplainStatement::ToString() const { return "explain statement"; }

} // namespace hybridsearch