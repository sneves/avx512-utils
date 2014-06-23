/*
    VPTERNLOG truth table generator
     
    Written in 2014 by Samuel Neves <sneves@dei.uc.pt>
     
    To the extent possible under law, the author(s) have dedicated all copyright
    and related and neighboring rights to this software to the public domain
    worldwide. This software is distributed without any warranty.
     
    You should have received a copy of the CC0 Public Domain Dedication along with
    this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#pragma once
#ifndef TERNLOG_HPP
#define TERNLOG_HPP

#include <type_traits>
#include <initializer_list>

#if defined(__AVX512F__)
#include <immintrin.h>
#endif

namespace ternlog 
{
    namespace detail 
    {
        template<typename T> struct Base {};

        template<unsigned Value>
        struct Bool : Base<Bool<Value>>
        {
            template<typename...Ts>
            constexpr unsigned operator()(Ts...) const { return 1&Value; }
        };

        template<unsigned Index>
        struct BoolArg : Base<BoolArg<Index>>
        {        
            template<typename...Ts>
            constexpr auto operator()(Ts...ts) const 
                -> typename std::common_type<Ts...>::type  
            {
                using T = typename std::common_type<Ts...>::type;
                return 1&begin(std::initializer_list<T>{ts...})[Index];
            }
        };


#define BinaryExpr(Name, Op)                                \
        template<typename T1, typename T2>                  \
        struct Name : Base<Name<T1,T2>>                     \
        {                                                   \
            template<typename...Ts>                         \
            constexpr auto operator()(Ts...ts) const        \
                -> decltype(1&(T1{}(ts...) Op T2{}(ts...))) \
            {                                               \
                return 1&(T1{}(ts...) Op T2{}(ts...));      \
            }                                               \
        };

        BinaryExpr(XorExpr, ^)
        BinaryExpr(AndExpr, &)
        BinaryExpr( OrExpr, |)
#undef BinaryExpr 

        template<typename T>
        struct NotExpr : Base<NotExpr<T>>
        {
            template<typename...Ts>
            constexpr auto operator()(Ts...ts) const 
                -> decltype(1&~T{}(ts...))
            { 
                return 1&~T{}(ts...); 
            }
        };

#define BinaryOperator(Type, Op)                                    \
        template<typename T1, typename T2>                          \
        static constexpr Type<T1, T2> operator Op(Base<T1> const&,  \
                                                  Base<T2> const&)  \
        {                                                           \
            return Type<T1, T2>{};                                  \
        }                                                                            

        BinaryOperator(XorExpr, ^)
        BinaryOperator(AndExpr, &)
        BinaryOperator( OrExpr, |)
#undef BinaryOperator

        template<typename T>
        static constexpr NotExpr<T> operator~(Base<T> const&)
        {
            return NotExpr<T>{};
        }

        template<typename F>
        static constexpr unsigned make_constant(F f)
        {
            return    f(0,0,0) +  2*f(0,0,1) +  4*f(0,1,0) +   8*f(0,1,1) +
                   16*f(1,0,0) + 32*f(1,0,1) + 64*f(1,1,0) + 128*f(1,1,1);
        }

#if defined(__AVX512F__)
        template<typename F>
        static inline __m512i _mm512_boolean_epi32(F f, __m512i a, 
                                                        __m512i b, 
                                                        __m512i c)
        {
            return _mm512_ternarylogic_epi32(a, b, c, make_constant(f));
        }

        template<typename F>
        static inline __m512i _mm512_boolean_epi64(F f, __m512i a, 
                                                        __m512i b, 
                                                        __m512i c)
        {
            return _mm512_ternarylogic_epi64(a, b, c, make_constant(f));
        }
#endif
    } // detail

    namespace placeholders 
    {
        constexpr detail::BoolArg<0> _a{};
        constexpr detail::BoolArg<1> _b{};
        constexpr detail::BoolArg<2> _c{};
        constexpr detail::Bool<1>    _1{};
        constexpr detail::Bool<0>    _0{};
    }

    using detail::make_constant;
#if defined(__AVX512F__)
    using detail::_mm512_boolean_epi32;
    using detail::_mm512_boolean_epi64;
#endif
} // ternlog

#endif

