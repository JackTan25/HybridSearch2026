

module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <cppjieba/Jieba.hpp>
#pragma clang diagnostic pop

export module jieba;

export namespace cppjieba {

using cppjieba::Jieba;
using cppjieba::Word;

} // namespace cppjieba
