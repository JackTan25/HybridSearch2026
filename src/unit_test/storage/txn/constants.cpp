

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import stl;
import global_resource_usage;
import third_party;
import logger;
import txn_state;
import hybridsearch_context;

using namespace hybridsearch;

class TxnContants : public BaseTest {};

TEST_F(TxnContants, test1) {
    using namespace hybridsearch;
    EXPECT_EQ(TxnState2Str(TxnState::kStarted), "Started");
}
