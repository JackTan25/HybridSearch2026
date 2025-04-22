

#include "gtest/gtest.h"
import base_test;

import hybridsearch_context;
import hybridsearch_exception;

import stl;
import global_resource_usage;
import third_party;
import logger;
import table_def;
import value;

import data_block;
import default_values;
import txn_manager;
import txn;
import status;
import background_process;
import bg_task;

using namespace hybridsearch;

class BGProcessTest : public BaseTestParamStr {};

INSTANTIATE_TEST_SUITE_P(TestWithDifferentParams, BGProcessTest, ::testing::Values(BaseTestParamStr::NULL_CONFIG_PATH));

TEST_P(BGProcessTest, test1) {
    using namespace hybridsearch;

    BGTaskProcessor processor(hybridsearch::hybridsearchContext::instance().storage()->wal_manager(), nullptr);

    processor.Start();

    processor.Stop();
}
