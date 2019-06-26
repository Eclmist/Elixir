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

#include "random.h"

exrBEGIN_NAMESPACE

exrVector3 Random::RandomInUnitSphere()
{
    exrVector3 p;

    do {
        p = 2.0f * exrVector3(Random::Uniform01(), Random::Uniform01(), Random::Uniform01()) - exrVector3(1.0f);
    } while (p.SquareMagnitude() >= 1.0f);

    return p;
}

exrVector3 Random::RandomOnUnitSphere()
{
    return RandomInUnitSphere().Normalized();
}


exrEND_NAMESPACE