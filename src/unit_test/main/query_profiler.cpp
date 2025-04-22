

#include "gtest/gtest.h"
import base_test;

import stl;
import profiler;
import hybridsearch_context;
import global_resource_usage;
import hybridsearch_exception;

using namespace hybridsearch;
class QueryProfilerTest : public BaseTest {
    void SetUp() override {
        RemoveDbDirs();
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

TEST_F(QueryProfilerTest, test1) {
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kParser), "Parser");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kLogicalPlan), "LogicalPlan");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kOptimizer), "Optimizer");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kPhysicalPlan), "PhysicalPlan");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kPipelineBuild), "PipelineBuild");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kTaskBuild), "TaskBuild");
    EXPECT_EQ(hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kExecution), "Execution");
    try {
        hybridsearch::QueryProfiler::QueryPhaseToString(hybridsearch::QueryPhase::kInvalid);
    } catch (std::exception &e) {
        std::string result(e.what());
        auto sub = hybridsearch::GetErrorMsg(result);
        EXPECT_EQ(sub, "Invalid query phase in query profiler");
    }
}

// Exception cases
TEST_F(QueryProfilerTest, test2) {
    hybridsearch::QueryProfiler profiler;
    try {
        profiler.StopPhase(hybridsearch::QueryPhase::kParser);
    } catch (std::exception &e) {
        std::string result(e.what());
        auto sub = hybridsearch::GetErrorMsg(result);
        EXPECT_EQ(sub, "Executor Error: Query phase isn't started, yet");
    }

    try {
        profiler.StartPhase(hybridsearch::QueryPhase::kParser);
        profiler.StartPhase(hybridsearch::QueryPhase::kLogicalPlan);

    } catch (std::exception &e) {
        std::string result(e.what());
        auto sub = hybridsearch::GetErrorMsg(result);
        EXPECT_EQ(sub, "Executor Error: Can't start new query phase before current phase(Parser) is finished");
    }
}

TEST_F(QueryProfilerTest, test3) {
    hybridsearch::QueryProfiler profiler;
    profiler.StartPhase(hybridsearch::QueryPhase::kParser);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kParser);
    profiler.StartPhase(hybridsearch::QueryPhase::kLogicalPlan);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kLogicalPlan);
    profiler.StartPhase(hybridsearch::QueryPhase::kOptimizer);
    profiler.optimizer().StartRule("OptimizerRule1");
    profiler.optimizer().StopRule();
    profiler.optimizer().StartRule("OptimizerRule2");
    profiler.optimizer().StopRule();
    profiler.optimizer().StartRule("OptimizerRule3");
    profiler.optimizer().StopRule();
    profiler.optimizer().StartRule("OptimizerRule4");
    profiler.optimizer().StopRule();
    profiler.StopPhase(hybridsearch::QueryPhase::kOptimizer);
    profiler.StartPhase(hybridsearch::QueryPhase::kPhysicalPlan);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kPhysicalPlan);
    profiler.StartPhase(hybridsearch::QueryPhase::kPipelineBuild);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kPipelineBuild);
    profiler.StartPhase(hybridsearch::QueryPhase::kTaskBuild);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kTaskBuild);
    profiler.StartPhase(hybridsearch::QueryPhase::kExecution);
    usleep(1000);
    profiler.StopPhase(hybridsearch::QueryPhase::kExecution);

    //    std::cout << profiler.ToString() << std::endl;
}