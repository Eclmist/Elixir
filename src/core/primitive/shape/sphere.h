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

#include "shape.h"

exrBEGIN_NAMESPACE

//! @brief A class that defines a sphere shape.
//! 
//! A class that defines a sphere shape and handles ray-sphere interactions
class Sphere: public Shape
{
public:
    //! @brief Constructs a sphere with a center, scale and material
    //! @param center           The origin of the sphere in world space
    //! @param radius           The radius of the sphere
    //! @param material         The material of the sphere
    Sphere(exrFloat radius);

    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;
    exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const override;

protected:
    AABB ComputeBoundingVolume() const override;

private:
    //! The radius of the sphere
    exrFloat m_Radius;

};

exrEND_NAMESPACE