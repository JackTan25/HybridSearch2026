

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;
import status;

using namespace hybridsearch;
class ExceptionTest : public BaseTest {};

TEST_F(ExceptionTest, all_exception_cases) {
    using namespace hybridsearch;

    try {
        throw hybridsearch::UnrecoverableException("unrecoverable error");
    } catch (std::exception &e) {
        EXPECT_EQ(std::string(e.what()), "unrecoverable error");
    }

    try {
        throw hybridsearch::RecoverableException(Status::UnexpectedError("recoverable error"));
    } catch (std::exception &e) {
        EXPECT_EQ(std::string(e.what()), "Unexpected error: recoverable error");
    }
}