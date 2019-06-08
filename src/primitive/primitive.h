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
#include "light/arealight.h"
#include "material/material.h"
#include "shape/shape.h"

exrBEGIN_NAMESPACE

class AABB;

//! Bridges the geometry processing and the shading subsystems
class Primitive
{
public:
    //! @brief Returns the bounding volume of the primitive
    //! @return                 The bounding volume of the primitive
    virtual AABB GetBoundingVolume() const = 0;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, and outputs the surface
    //! intersection data in <interaction> 
    //!
    //! @param ray              The ray to test against
    //! @param interaction      The output surface interaction struct
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const = 0;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, but ignore surface data.
    //! Useful for shadow rays that don't require surface data.
    //!
    //! @param ray              The ray to test against
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool HasIntersect(const Ray& r) const = 0;

    //! Returns the area light pointer if the primitive is a light source.
    //! Returns nullptr otherwise.
    virtual const AreaLight* GetAreaLight() const = 0;

    //! Returns the material of the current primitive
    virtual const Material* GetMaterial() const = 0;
};

class GeometricPrimitive : public Primitive
{
public:
    virtual AABB GetBoundingVolume() const override;
    virtual exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const override;
    virtual exrBool HasIntersect(const Ray& r) const override;

public:
    virtual inline const AreaLight* GetAreaLight() const override { return m_AreaLight.get(); };
    virtual inline const Material* GetMaterial() const override { return m_Material.get(); };

public:
    std::unique_ptr<Shape> m_Shape;
    std::unique_ptr<Material> m_Material;
    std::unique_ptr<AreaLight> m_AreaLight;
};

exrEND_NAMESPACE