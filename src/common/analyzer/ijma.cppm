

module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <ijma.h>
#include <mecab_wrapper.h>
#pragma clang diagnostic pop

export module ijma;

export namespace jma {

using jma::Analyzer;
using jma::Knowledge;
using jma::MeCab;
using jma::Sentence;
} // namespace jma
