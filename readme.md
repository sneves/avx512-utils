This is a small header file (ternlog.hpp) that, when included, allows 
VPTERNLOG immediates to be computed at compile-time. 

_mm512_boolean_epi{32,64} translates a boolean expression using _a, _b, _c, 
_0, _1 (in the ternlog::placeholders namespace) as variables and constants, 
and translates it into a _mm512_ternarylogic_epi{32, 64} call with the 
appropriate immediate. See example.cpp for an example of use.