/*
    VPTERNLOG truth table generator

    Written in 2014 by Samuel Neves <sneves@dei.uc.pt>

    To the extent possible under law, the author(s) have dedicated all copyright
    and related and neighboring rights to this software to the public domain
    worldwide. This software is distributed without any warranty.

    You should have received a copy of the CC0 Public Domain Dedication along with
    this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#ifndef TERNLOG_HPP
#define TERNLOG_HPP

#if defined(__AVX512F__)
#include <immintrin.h>
#endif

namespace ternlog {
namespace detail {
template <int X> struct boolean {};

#define DefineUnaryOperator(Op)                                                \
  template<int X> constexpr boolean<Op X> operator Op(boolean<X>) {            \
    return {};                                                                 \
  }                                                                            \

DefineUnaryOperator(~)
#undef DefineUnaryOperator

#define DefineBinaryOperator(Op)                                               \
  template<int X, int Y>                                                       \
  constexpr boolean<X Op Y> operator Op(boolean<X>, boolean<Y>) {              \
    return {};                                                                 \
  }

DefineBinaryOperator(^)
DefineBinaryOperator(&)
DefineBinaryOperator(|)
#undef DefineBinaryOperator

template<int X>
constexpr int make_constant(boolean<X>) {
  return X & 0xff;
}

#if defined(__AVX512F__)
template<int X>
inline __m512i _mm512_boolean_epi32(__m512i a, __m512i b, __m512i c,
                                    boolean<X>) {
  return _mm512_ternarylogic_epi32(a, b, c, X & 0xff);
}

template<int X>
inline __m512i _mm512_mask_boolean_epi32(__m512i src, __mmask16 k, __m512i a,
                                         __m512i b, boolean<X>) {
  return _mm512_mask_ternarylogic_epi32(src, k, a, b, X & 0xff);
}

template<int X>
inline __m512i _mm512_maskz_boolean_epi32(__mmask16 k, __m512i a, __m512i b,
                                          __m512i c, boolean<X>) {
  return _mm512_maskz_ternarylogic_epi32(k, a, b, c, X & 0xff);
}

template<int X>
inline __m512i _mm512_boolean_epi64(__m512i a, __m512i b, __m512i c,
                                    boolean<X>) {
  return _mm512_ternarylogic_epi64(a, b, c, X & 0xff);
}

template<int X>
inline __m512i _mm512_mask_boolean_epi64(__m512i src, __mmask8 k, __m512i a,
                                         __m512i b, boolean<X>) {
  return _mm512_mask_ternarylogic_epi64(src, k, a, b, X & 0xff);
}

template<int X>
inline __m512i _mm512_maskz_boolean_epi64(__mmask8 k, __m512i a, __m512i b,
                                          __m512i c, boolean<X>) {
  return _mm512_maskz_ternarylogic_epi64(k, a, b, c, X & 0xff);
}
#endif

} // detail

inline namespace placeholders {
constexpr detail::boolean<0xf0> _a{};
constexpr detail::boolean<0xcc> _b{};
constexpr detail::boolean<0xaa> _c{};
constexpr detail::boolean<0xff> _1{};
constexpr detail::boolean<0x00> _0{};
}

using detail::make_constant;
#if defined(__AVX512F__)
using detail::_mm512_boolean_epi32;
using detail::_mm512_mask_boolean_epi32;
using detail::_mm512_maskz_boolean_epi32;
using detail::_mm512_boolean_epi64;
using detail::_mm512_mask_boolean_epi64;
using detail::_mm512_maskz_boolean_epi64;
#endif
} // ternlog

#endif
