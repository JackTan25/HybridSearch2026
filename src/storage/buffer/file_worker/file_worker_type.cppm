

module;

export module file_worker_type;

import stl;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

export enum class FileWorkerType {
    kDataFile,
    kVarFile,
    kIVFIndexFile,
    kHNSWIndexFile,
    kRawFile,
    kSecondaryIndexFile,
    kVersionDataFile,
    kIndexFile,
    kEMVBIndexFile,
    kBMPIndexFile,
    kInvalid,
};

export String FileWorkerType2Str(FileWorkerType type) {
    switch (type) {
        case FileWorkerType::kDataFile: {
            return "data";
        }
        case FileWorkerType::kVarFile: {
            return "var";
        }
        case FileWorkerType::kIVFIndexFile: {
            return "IVF index";
        }
        case FileWorkerType::kHNSWIndexFile: {
            return "HNSW index";
        }
        case FileWorkerType::kRawFile: {
            return "raw";
        }
        case FileWorkerType::kSecondaryIndexFile: {
            return "secondary index";
        }
        case FileWorkerType::kVersionDataFile: {
            return "version data";
        }
        case FileWorkerType::kIndexFile: {
            return "index";
        }
        case FileWorkerType::kEMVBIndexFile: {
            return "EMVB index";
        }
        case FileWorkerType::kBMPIndexFile: {
            return "BMP index";
        }
        case FileWorkerType::kInvalid: {
            String error_message = "Invalid file worker type";
            UnrecoverableError(error_message);
        }
    }
    String error_message = "Unreachable";
    UnrecoverableError(error_message);
    return error_message;
}

} // namespace hybridsearch
