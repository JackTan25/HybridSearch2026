

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import stl;
import hybridsearch_context;
import sql_runner;

using namespace hybridsearch;
class FragmentTest : public BaseTest {
    void SetUp() override {
        BaseTest::SetUp();
        BaseTest::RemoveDbDirs();
#ifdef hybridsearch_DEBUG
        hybridsearch::GlobalResourceUsage::Init();
#endif
        std::shared_ptr<std::string> config_path = nullptr;
        hybridsearch::hybridsearchContext::instance().InitPhase1(config_path);
        hybridsearch::hybridsearchContext::instance().InitPhase2();
    }

    void TearDown() override {
        hybridsearch::hybridsearchContext::instance().UnInit();
#ifdef hybridsearch_DEBUG
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(hybridsearch::GlobalResourceUsage::GetRawMemoryCount(), 0);
        hybridsearch::GlobalResourceUsage::UnInit();
#endif
        BaseTest::TearDown();
    }
};

TEST_F(FragmentTest, test_build_fragment) {
    using namespace hybridsearch;
    /// DDL
    auto result0 = SQLRunner::Run("create table t1(a bigint)", true);
    EXPECT_EQ(result0->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result1 = SQLRunner::Run("create database db1", true);
    EXPECT_EQ(result1->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result2 = SQLRunner::Run("create table db1.t1(a bigint)", true);
    EXPECT_EQ(result2->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result3 = SQLRunner::Run("create table t2(a bigint)", true);
    EXPECT_EQ(result3->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result4 = SQLRunner::Run("create table t3(c1 embedding(bit,10))", true);
    EXPECT_EQ(result4->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result5 = SQLRunner::Run("drop database db1", true);
    EXPECT_EQ(result5->definition_ptr_.get()->columns()[0]->name_, "OK");

    /// SPJ
    //    SQLRunner::Run("select * from t1 where a = 1", true);
    //    SQLRunner::Run("select a+1 from t1", true);

    /// DDL
    auto result6 = SQLRunner::Run("drop table t1", true);
    EXPECT_EQ(result6->definition_ptr_.get()->columns()[0]->name_, "OK");

    /// Show
    auto result7 = SQLRunner::Run("show tables", true);
    EXPECT_EQ(result7->definition_ptr_->column_count(), 9u);
    auto result8 = SQLRunner::Run("show table t2", true);
    EXPECT_EQ(result8->definition_ptr_->column_count(), 2u);

    /// DDL
    auto result9 = SQLRunner::Run("drop table t3", true);
    EXPECT_EQ(result9->definition_ptr_.get()->columns()[0]->name_, "OK");
    auto result10 = SQLRunner::Run("drop table t2", true);
    EXPECT_EQ(result9->definition_ptr_.get()->columns()[0]->name_, "OK");
}
