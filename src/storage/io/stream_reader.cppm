

module;

#include <fstream>

export module stream_reader;

import stl;
import status;

namespace hybridsearch {

export class StreamReader {

public:
    StreamReader() = default;
    ~StreamReader();

    Status Init(const String &file_name);
    bool ReadLine(String &line);
    void Close();

private:
    std::ifstream file_;
};

} // namespace hybridsearch