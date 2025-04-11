

module;

export module sql_runner;

import stl;
import data_table;

namespace hybridsearch {

export class SQLRunner {

public:
    static SharedPtr<DataTable> Run(const String &sql_text, bool print = true);
};

} // namespace hybridsearch
