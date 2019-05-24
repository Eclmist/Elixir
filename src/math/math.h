#pragma once

#define EXR_M_PI                3.141592653589793F
#define EXR_EPSILON             0.00001f
#define EXR_MAX_FLOAT           (3.402823466e+38F)
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