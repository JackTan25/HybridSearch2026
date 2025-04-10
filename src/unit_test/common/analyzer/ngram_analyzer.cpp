

#include "gtest/gtest.h"
import base_test;

import stl;
import term;
import ngram_analyzer;
import standard_analyzer;

using namespace hybridsearch;

class NGramAnalyzerTest : public BaseTest {};

TEST_F(NGramAnalyzerTest, test1) {
    NGramAnalyzer analyzer(2);
    TermList term_list;
    String input("hello world  123");
    analyzer.Analyze(input, term_list);

    ASSERT_EQ(term_list.size(), 10U);
    ASSERT_EQ(term_list[0].text_, String("he"));
    ASSERT_EQ(term_list[0].word_offset_, 0U);
    ASSERT_EQ(term_list[1].text_, String("el"));
    ASSERT_EQ(term_list[1].word_offset_, 1U);
    ASSERT_EQ(term_list[2].text_, String("ll"));
    ASSERT_EQ(term_list[2].word_offset_, 2U);
    ASSERT_EQ(term_list[3].text_, String("lo"));
    ASSERT_EQ(term_list[3].word_offset_, 3U);
    ASSERT_EQ(term_list[4].text_, String("wo"));
    ASSERT_EQ(term_list[4].word_offset_, 4U);
    ASSERT_EQ(term_list[5].text_, String("or"));
    ASSERT_EQ(term_list[5].word_offset_, 5U);
    ASSERT_EQ(term_list[6].text_, String("rl"));
    ASSERT_EQ(term_list[6].word_offset_, 6U);
    ASSERT_EQ(term_list[7].text_, String("ld"));
    ASSERT_EQ(term_list[7].word_offset_, 7U);
    ASSERT_EQ(term_list[8].text_, String("12"));
    ASSERT_EQ(term_list[8].word_offset_, 8U);
    ASSERT_EQ(term_list[9].text_, String("23"));
    ASSERT_EQ(term_list[9].word_offset_, 9U);
}

TEST_F(NGramAnalyzerTest, test2) {
    NGramAnalyzer analyzer(1);
    TermList term_list;
    String input("abc de  fg");
    analyzer.Analyze(input, term_list);

    ASSERT_EQ(term_list.size(), 7U);
    ASSERT_EQ(term_list[0].text_, String("a"));
    ASSERT_EQ(term_list[0].word_offset_, 0U);
    ASSERT_EQ(term_list[1].text_, String("b"));
    ASSERT_EQ(term_list[1].word_offset_, 1U);
    ASSERT_EQ(term_list[2].text_, String("c"));
    ASSERT_EQ(term_list[2].word_offset_, 2U);
    ASSERT_EQ(term_list[3].text_, String("d"));
    ASSERT_EQ(term_list[3].word_offset_, 3U);
    ASSERT_EQ(term_list[4].text_, String("e"));
    ASSERT_EQ(term_list[4].word_offset_, 4U);
    ASSERT_EQ(term_list[5].text_, String("f"));
    ASSERT_EQ(term_list[5].word_offset_, 5U);
    ASSERT_EQ(term_list[6].text_, String("g"));
    ASSERT_EQ(term_list[6].word_offset_, 6U);
}