

#include "gtest/gtest.h"
import base_test;

import stl;
import physical_explain;

using namespace hybridsearch;
class ExplainTest : public BaseTest {};

TEST_F(ExplainTest, test1) {
    using namespace hybridsearch;

    Vector<SharedPtr<String>> fragment_text;
    Vector<SharedPtr<String>> task_text;

    fragment_text.push_back(MakeShared<String>("FRAGMENT (1)"));
    fragment_text.push_back(MakeShared<String>("-> SINK (7)"));
    fragment_text.push_back(MakeShared<String>("-> PROJECT (4)"));
    fragment_text.push_back(MakeShared<String>("   - table index: #4"));
    fragment_text.push_back(MakeShared<String>("   - expressions: [c1 (#0)]"));
    fragment_text.push_back(MakeShared<String>("-> AGGREGATE (3)"));
    fragment_text.push_back(MakeShared<String>("   - aggregate table index: #3"));
    fragment_text.push_back(MakeShared<String>("   - aggregate: []"));
    fragment_text.push_back(MakeShared<String>("   - group by table index: #2"));
    fragment_text.push_back(MakeShared<String>("   - group by: [c1 (#0)]"));
    fragment_text.push_back(MakeShared<String>("-> SOURCE (11): FRAGMENT #2"));
    fragment_text.push_back(MakeShared<String>());

    fragment_text.push_back(MakeShared<String>("FRAGMENT (2)"));
    fragment_text.push_back(MakeShared<String>("-> SINK (9)"));
    fragment_text.push_back(MakeShared<String>("-> TABLE SCAN (2)"));
    fragment_text.push_back(MakeShared<String>("   - table name: t1(default_db.t1)"));
    fragment_text.push_back(MakeShared<String>("   - table index: #1"));
    fragment_text.push_back(MakeShared<String>("   - output_columns: [c1]"));
    fragment_text.push_back(MakeShared<String>("-> SOURCE (10)"));
    fragment_text.push_back(MakeShared<String>());

    task_text.push_back(MakeShared<String>("Fragment #1 * 4 Task"));
    task_text.push_back(MakeShared<String>("-> Task #0"));
    task_text.push_back(MakeShared<String>("-> Task #1"));
    task_text.push_back(MakeShared<String>("-> Task #2"));
    task_text.push_back(MakeShared<String>("-> Task #3"));
    task_text.push_back(MakeShared<String>());

    task_text.push_back(MakeShared<String>("Fragment #2 * 2 Task"));
    task_text.push_back(MakeShared<String>("-> Task #0"));
    task_text.push_back(MakeShared<String>("-> Task #1"));
    task_text.push_back(MakeShared<String>());

    PhysicalExplain::AlignParagraphs(fragment_text, task_text);

    EXPECT_EQ(fragment_text.size(), task_text.size());
}