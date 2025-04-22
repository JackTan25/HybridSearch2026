

#pragma once

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#elif defined(__GNUC__) && defined(__aarch64__)
#include <simde/x86/avx512.h>
#define __SSE2__
#endif
