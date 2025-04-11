

module;

#include <fstream>

module stream_reader;

import stl;
import logger;
import status;
import hybridsearch_exception;
import third_party;

namespace hybridsearch {

StreamReader::~StreamReader() { Close(); }

Status StreamReader::Init(const String &file_name) {
    file_.open(file_name);
    if (!file_.is_open()) {
        return Status::IOError(fmt::format("{} can't open", file_name));
    }
    return Status::OK();
}

bool StreamReader::ReadLine(String &line) {
    if (getline(file_, line)) {
        return true;
    } else {
        return false;
    }
}

void StreamReader::Close() { file_.close(); }

} // namespace hybridsearch