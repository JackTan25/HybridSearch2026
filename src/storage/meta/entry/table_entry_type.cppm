

module;

export module table_entry_type;

import stl;
import logger;
import hybridsearch_exception;

namespace hybridsearch {

export enum TableEntryType : i8 {
    kTableEntry,
    kCollectionEntry,
};

export inline String ToString(TableEntryType type) {
    switch (type) {
        case TableEntryType::kTableEntry: {
            return "Table";
        }
        case TableEntryType::kCollectionEntry: {
            return "Collection";
        }
        default: {
            String error_message = "Invalid base table type";
            UnrecoverableError(error_message);
        }
    }
}

} // namespace hybridsearch
