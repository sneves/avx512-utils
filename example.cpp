
#include "ternlog.hpp"

namespace MD5 {
using namespace ternlog;

__m512i F(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, (_a & _b) | (~_a & _c));
}

__m512i G(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, (_a & _c) | (_b & ~_c));
}

__m512i H(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, _a ^ _b ^ _c);
}

__m512i I(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, _b ^ (_a | ~_c));
}
}

namespace SHA2 {
using namespace ternlog;

__m512i Maj(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, (_a & _b) | (_a & _c) | (_b & _c));
}

__m512i Ch(__m512i a, __m512i b, __m512i c) {
  return _mm512_boolean_epi64(a, b, c, (_a & _b) | (~_a & _c));
}
}