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

#include "ray.h"

exrBEGIN_NAMESPACE

class RayDifferential : public Ray
{
public:
    RayDifferential()
        : Ray()
        , m_HasDifferentials(false) {};

    RayDifferential(const exrPoint3& origin, const exrVector3& direction, exrFloat tmax = MaxFloat)
        : Ray(origin, direction, tmax)
        , m_HasDifferentials(false) {};

    RayDifferential(const Ray& ray)
        : Ray(ray)
        , m_HasDifferentials(false) {};

    void ScaleDifferentials(exrFloat scale);

public:
    exrBool m_HasDifferentials;
    exrPoint3 m_RxOrigin;
    exrPoint3 m_RyOrigin;
    exrVector3 m_RxDirection;
    exrVector3 m_RyDirection;
};

exrEND_NAMESPACE