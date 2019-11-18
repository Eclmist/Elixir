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

#include <random>
#include "system/system.h"
#include "math/math.h"

exrBEGIN_NAMESPACE

//! @brief A class that handles various pseudo random value generations
class Random
{
public:
    static void Seed(exrU32 seed) { m_Rng = std::mt19937(seed); }

    //! @return A random number in the range [0, 2^23 - 1].
    static exrU32 UniformUInt32() { return m_Rng(); }
    
    //! @param b the upper bound of the generated number
    //! @return A random number in the range [0, b].
    static exrU32 UniformUInt32(exrU32 b) {
        return (std::uniform_int_distribution<exrU32>(0, b))(m_Rng);
    }

    //! @return A random floating-point number in the range [0,1).
    static exrFloat UniformFloat() {
        return (std::uniform_real_distribution<exrFloat>(0, 1))(m_Rng);
    }

private:
    Random() {}
    static std::mt19937 m_Rng;
};

exrEND_NAMESPACE