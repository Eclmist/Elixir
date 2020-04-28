/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

class Scene;

//! A closure that allows light to return a radiance under the assumption that the
//! reference point and light source are mutually visible. This can allow us to
//! ignore shadow rays if the illumination is irrelevant.
class VisibilityTester
{
public:
    VisibilityTester() {};
    VisibilityTester(const Interaction &p0, const Interaction& p1)
        : m_P0(p0)
        , m_P1(p1) {};

    exrBool IsOccluded(const Scene& scene) const;

public:
    Interaction m_P0;
    Interaction m_P1;
};

exrEND_NAMESPACE
