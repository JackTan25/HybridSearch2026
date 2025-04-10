
#if 0
#include "gtest/gtest.h"
import base_test;

import stl;
import user_defined_analyzer;
import compilation_config;
import status;
import python_instance;

using namespace hybridsearch;

class UserDefinedAnalyzerTest : public BaseTest {};

TEST_F(UserDefinedAnalyzerTest, test1) {
    PythonInstance::Init();

    UserDefinedAnalyzer analyzer(std::string(test_data_path()) + "/scripts/uda1.py");
    auto [term_vector, status] = analyzer.Analyze("text1 text2 text3");
    EXPECT_TRUE(status.ok());
    EXPECT_STREQ(term_vector[0].c_str(), "text1");
    EXPECT_STREQ(term_vector[1].c_str(), "text2");
    EXPECT_STREQ(term_vector[2].c_str(), "text3");

    PythonInstance::UnInit();
}



TEST_F(UserDefinedAnalyzerTest, test2) {
    PythonInstance::Init();

    SizeT thread_count = 1;
    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (SizeT i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread([&] {
            UserDefinedAnalyzer analyzer(std::string(test_data_path()) + "/scripts/uda1.py");
            auto [term_vector, status] = analyzer.Analyze("text1 text2 text3");
            EXPECT_TRUE(status.ok());
            EXPECT_STREQ(term_vector[0].c_str(), "text1");
            EXPECT_STREQ(term_vector[1].c_str(), "text2");
            EXPECT_STREQ(term_vector[2].c_str(), "text3");
        }));
    }

    for (SizeT i = 0; i < thread_count; ++i) {
        threads[i].join();
    }

    PythonInstance::UnInit();
}

TEST_F(UserDefinedAnalyzerTest, test3) {
    PythonInstance::Init();
    UserDefinedAnalyzer analyzer(std::string(test_data_path()) + "/scripts/uda2.py");
    auto [term_vector, status] = analyzer.Analyze("");
    EXPECT_EQ(status.code(), ErrorCode::kFailToRunPython);
    PythonInstance::UnInit();
}

TEST_F(UserDefinedAnalyzerTest, test4) {
    PythonInstance::Init();
    UserDefinedAnalyzer analyzer(std::string(test_data_path()) + "/scripts/uda3.py");
    auto [term_vector, status] = analyzer.Analyze("");
    EXPECT_EQ(status.code(), ErrorCode::kFailToRunPython);
    PythonInstance::UnInit();
}

TEST_F(UserDefinedAnalyzerTest, test5) {
    PythonInstance::Init();

    UserDefinedAnalyzer analyzer(std::string(test_data_path()) + "/scripts/uda4.py");

    auto [term_vector, status] = analyzer.Analyze("text1 text2 text3");
    EXPECT_EQ(status.code(), ErrorCode::kFailToRunPython);

    PythonInstance::UnInit();
}
#endif
