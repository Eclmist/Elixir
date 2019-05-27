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

#include "core/elixir.h"

exrBEGIN_NAMESPACE

//! @brief A class that handles various pseudo random value generations
class Random
{
public:
    //! @brief Seeds the random class
    //! @param a                The value to seed the random class
    static inline void Seed(exrU32 a) { srand(a); }

    //! @brief Generates a random value between 0 and 1
    //! @return A random exrFloat between 0 and 1
    static inline exrFloat Random01() { return (exrFloat)rand() / RAND_MAX; }

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit sphere
    static exrVector3 RandomInUnitSphere();

    //! @brief Generates a vector on the surface of a unit sphere
    //! @return A random vector on the surface of a unit sphere
    static exrVector3 RandomOnUnitSphere();

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit disc
    static exrVector3 RandomInUnitDisc();
};

exrEND_NAMESPACE