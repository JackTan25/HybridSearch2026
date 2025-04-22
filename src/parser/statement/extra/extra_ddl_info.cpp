

#include "extra_ddl_info.h"
#include "parser_assert.h"

namespace hybridsearch {

std::string ConflictTypeToStr(ConflictType type) {
    switch (type) {
        case ConflictType::kInvalid: {
            ParserError("Invalid conflict type");
            return "Invalid";
        }
        case ConflictType::kIgnore: {
            return "Ignore";
        }
        case ConflictType::kError: {
            return "Issue error";
        }
        case ConflictType::kReplace: {
            return "Replace";
        }
    }
    ParserError("Unreachable");
}

} // namespace hybridsearch