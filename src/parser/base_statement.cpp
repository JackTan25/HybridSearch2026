

#include "base_statement.h"
#include <string>

namespace hybridsearch {

std::string StatementType2Str(StatementType statement_type) {
    switch (statement_type) {
        case StatementType::kInvalidStmt: {
            ParserError("Unexpected error.");
            break;
        }
        case StatementType::kSelect: {
            return "SELECT";
        }
        case StatementType::kCopy: {
            return "COPY";
        }
        case StatementType::kInsert: {
            return "INSERT";
        }
        case StatementType::kUpdate: {
            return "UPDATE";
        }
        case StatementType::kDelete: {
            return "DELETE";
        }
        case StatementType::kCreate: {
            return "CREATE";
        }
        case StatementType::kDrop: {
            return "DROP";
        }
        case StatementType::kPrepare: {
            return "PREPARE";
        }
        case StatementType::kExecute: {
            return "EXECUTE";
        }
        case StatementType::kAlter: {
            return "ALTER";
        }
        case StatementType::kShow: {
            return "SHOW";
        }
        case StatementType::kExplain: {
            return "EXPLAIN";
        }
        case StatementType::kFlush: {
            return "FLUSH";
        }
        case StatementType::kOptimize: {
            return "OPTIMIZE";
        }
        case StatementType::kCommand: {
            return "COMMAND";
        }
        case StatementType::kCompact: {
            return "COMPACT";
        }
        case StatementType::kAdmin: {
            return "ADMIN";
        }
    }

    return "Error";
}

} // namespace hybridsearch
