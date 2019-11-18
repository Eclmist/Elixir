/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "system/system.h"

#define EXR_M_PI                3.141592653589793f
#define EXR_M_INVPI             0.31830988618f
#define EXR_M_INV2PI            0.15915494309f
#define EXR_M_INV4PI            0.07957747154f
#define EXR_M_PIOVER2           1.57079632679f
#define EXR_M_PIOVER4           0.78539816339f

#define EXR_EPSILON             0.0001f
#define EXR_ONE_MINUS_EPSILON   0.9999f

#define exrSaturate(x)          exrClamp(x, 0, 1)
#define exrPow(x, n)            pow(x, n)
#define exrDegToRad(x)          x * (EXR_M_PI / 180.0f)

exrBEGIN_NAMESPACE

static constexpr exrFloat MaxFloat = std::numeric_limits<float>::max();
static constexpr exrFloat MinFloat = std::numeric_limits<float>::min();
static constexpr exrFloat Infinity = std::numeric_limits<float>::infinity();

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

inline exrBool exrQuadratic(exrFloat a, exrFloat b, exrFloat c, exrFloat* t0, exrFloat* t1)
{
    exrFloat discriminator = b * b - 4 * a * c;

    if (discriminator < 0)
        return false;

    exrFloat droot = sqrt(discriminator);

    // Compute quadratic values
    *t0 = -0.5f * a * (b - droot);
    *t1 = -0.5f * a * (b + droot);

    if (*t0 > * t1) std::swap(*t0, *t1);
    return true;
}

exrEND_NAMESPACE