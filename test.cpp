#include <string.h>
#include <stdio.h>
#include <functional>
#include <iterator>
#include <algorithm>
#include <random>

#include "ternlog.hpp"

#if defined(__AVX512F__)
static unsigned char u0[64], u1[64], u2[64];
static unsigned char v0[64], v1[64];

template <typename F1, typename F2> static void check_function(F1 f1, F2 f2) {
  const __m512i r0 = _mm512_load_epi64(u0);
  const __m512i r1 = _mm512_load_epi64(u1);
  const __m512i r2 = _mm512_load_epi64(u2);

  const __m512i s0 = _mm512_boolean_epi64(r0, r1, r2, f1);
  const __m512i s1 = f2(r0, r1, r2);

  _mm512_store_epi64(v0, s0);
  _mm512_store_epi64(v1, s1);

  if (0 == memcmp(v0, v1, 64))
    puts("ok");
  else
    puts("fail");
}
#else
template <typename F> static void check_function(F f, unsigned x) {
  if (x == make_constant(f))
    puts("ok");
  else
    puts("fail");
}
#endif

int main() {
  using namespace ternlog;
#if defined(__AVX512F__)
  std::random_device rd;
  generate(std::begin(u0), std::end(u0), ref(rd));
  generate(std::begin(u1), std::end(u1), ref(rd));
  generate(std::begin(u2), std::end(u2), ref(rd));

  check_function(~_1, [](__m512i a, __m512i b,
                         __m512i c) { return _mm512_setzero_si512(); });

  check_function((~_a & _b) | (_a & ~_b), [](__m512i a, __m512i b, __m512i c) {
    return _mm512_xor_epi64(a, b);
  });

  check_function(_b ^ _c, [](__m512i a, __m512i b,
                             __m512i c) { return _mm512_xor_epi64(b, c); });

  check_function(_a & _b ^ _b & _c ^ _a & _c,
                 [](__m512i a, __m512i b, __m512i c) {
    return _mm512_xor_epi64(
        _mm512_xor_epi64(_mm512_and_epi64(a, b), _mm512_and_epi64(a, c)),
        _mm512_and_epi64(b, c));
  });

  check_function((~_b | ~_c) & (~_a ^ ~_b ^ _c),
                 [](__m512i a, __m512i b, __m512i c) {
    return _mm512_xor_epi64(_mm512_and_epi64(_mm512_and_epi64(a, b), c),
                            _mm512_xor_epi64(_mm512_xor_epi64(a, b), c));
  });
#else
  check_function(~_1, 0x00);
  check_function((~_a & _b) | (_a & ~_b), 0x3C);
  check_function(_b ^ _c, 0x66);
  check_function(_a & _b ^ _b & _c ^ _a & _c, 0xE8);
  check_function((~_b | ~_c) & (~_a ^ ~_b ^ _c), 0x16);
#endif
  return 0;
}
