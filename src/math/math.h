#ifndef __MATH_H__
#define __MATH_H__

#include <math.h>
#include "random.h"

#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

#define EXR_EPSILON             0.00001f
#define EXR_MAX_FLOAT           (FLT_MAX)
#define EXR_MIN_FLOAT           (-EXR_MAX_FLOAT)

#define exrSaturate(x)          exrClamp(x, 0, 1)
#define exrPow(x, n)            pow(x, n)

exrBEGIN_NAMESPACE

template <typename _Type>
inline _Type exrMax(_Type lhs, _Type rhs)
{
    return lhs >= rhs ? lhs : rhs;
}

template <typename _Type>
inline _Type exrMin(_Type lhs, _Type rhs)
{
    return lhs <= rhs ? lhs : rhs;
}

template <typename _Type>
inline _Type exrRound(_Type floatingVal)
{
    return (floatingVal > _Type(0)) ? exrS32(floatingVal + _Type(0.5)) : exrS32(floatingVal - _Type(0.5));
}

template <typename _Type1, typename _Type2, typename _Type3>
inline _Type1 exrClamp(_Type1 v, _Type2 a, _Type3 b)
{
    return v < a ? a : (v < b ? v : b);
}

template <typename _Type1, typename _Type2>
inline _Type1 exrLerp(_Type1 a, _Type1 b, _Type2 t)
{
    return ((static_cast<_Type2>(1) - t) * a + t * b);
}

exrEND_NAMESPACE

#endif __MATH_H__

