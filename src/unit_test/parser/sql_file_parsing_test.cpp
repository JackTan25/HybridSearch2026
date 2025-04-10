

#include "gtest/gtest.h"
import base_test;

import compilation_config;
import hybridsearch_context;
import hybridsearch_exception;

import global_resource_usage;
import third_party;
import sql_parser;
import logger;
import null_value;
import stl;
import parser_result;

using namespace hybridsearch;
class SQLFileParsingTest : public BaseTest {};

TEST_F(SQLFileParsingTest, tpch) {
    using namespace hybridsearch;

    SharedPtr<SQLParser> parser = MakeShared<SQLParser>();
    SharedPtr<ParserResult> result = MakeShared<ParserResult>();

    // Get all tpch sql text;
    String path = String(test_data_path()) + "/tpch";
    Vector<String> files = GetFilesFromDir(path);
    SizeT file_count = files.size();
    for (SizeT idx = 0; idx < file_count; ++idx) {
        const String &filename = files[idx];
        if (filename == "README.md") {
            continue;
        }
        std::ifstream t(filename);
        String input_sql((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        parser->Parse(input_sql, result.get());
        result->Reset();
    }
}

namespace hybridsearch {

void ReadSQLs(const String &file_path, Vector<String> &sqls) {
    std::ifstream infile(file_path);
    String line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        // Skip comments.
        if (line[0] == '#' || (line[0] == '-' && line[1] == '-')) {
            continue;
        }
        if (line[0] == '!') {
            line = line.substr(1);
        }
        sqls.emplace_back(line);
    }
}

} // namespace hybridsearch

TEST_F(SQLFileParsingTest, hyrise) {
    using namespace hybridsearch;

    SharedPtr<SQLParser> parser = MakeShared<SQLParser>();
    SharedPtr<ParserResult> result = MakeShared<ParserResult>();

    // Get all tpch sql text;
    String good_sql = String(test_data_path()) + "/hyrise/good.sql";
    String bad_sql = String(test_data_path()) + "/hyrise/bad.sql";
    Path good_sql_path(good_sql);
    Path bad_sql_path(bad_sql);

    Vector<String> sqls;
    ReadSQLs(good_sql_path, sqls);
    for (auto &input_sql : sqls) {
        //        std::cout << input_sql << std::endl;
        parser->Parse(input_sql, result.get());
        result->Reset();
    }

    ReadSQLs(bad_sql_path, sqls);
    for (auto &input_sql : sqls) {
        //        std::cout << input_sql << std::endl;
        parser->Parse(input_sql, result.get());
        result->Reset();
    }
}

TEST_F(SQLFileParsingTest, hybridsearch) {
    using namespace hybridsearch;

    SharedPtr<SQLParser> parser = MakeShared<SQLParser>();
    SharedPtr<ParserResult> result = MakeShared<ParserResult>();

    // Get all tpch sql text;
    String good_sql = String(test_data_path()) + "/hybridsearch/good.sql";
    Path good_sql_path(good_sql);

    Vector<String> sqls;
    ReadSQLs(good_sql_path, sqls);
    for (auto &input_sql : sqls) {
        //        std::cout << input_sql << std::endl;
        parser->Parse(input_sql, result.get());
        result->Reset();
    }
}
