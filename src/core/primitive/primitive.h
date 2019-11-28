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
#include "core/material/material.h"
#include "core/primitive/shape/shape.h"
#include "core/primitive/transform.h"

exrBEGIN_NAMESPACE

class AABB;

//! Bridges the geometry processing and the shading subsystems
class Primitive
{
public:
    //! @brief Returns the bounding volume of the primitive
    //! @return                 The bounding volume of the primitive
    AABB GetBoundingVolume() const;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, and outputs the surface
    //! intersection data in <interaction> 
    //!
    //! @param ray              The ray to test against
    //! @param interaction      The output surface interaction struct
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! Test the geometry for intersection with the ray, but ignore surface data.
    //! Useful for shadow rays that don't require surface data.
    //!
    //! @param ray              The ray to test against
    //! @param tHit             The t-value of the hitpoint along the ray
    //! 
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& r, exrFloat& tHit) const;

    //! @brief An overload for HasIntersect for when the result of tHit is not needed
    //! 
    //! @param ray              The ray to test against
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& r) const;

    //! @brief Sets the shape of the primitive
    //! 
    //! Assign a shape to the primitive. A reference to the primitive is also set in
    //! the shape object. Ownership of the shape will be transfered to the primitive
    //!
    //! @param shape            A pointer to the shape
    void SetShape(std::unique_ptr<Shape> shape);

    //! @brief Sets the transform of the primitive
    //! 
    //! Assign a transform to the primitive. Ownership of the shape will be 
    //! transfered to the primitive
    //!
    //! @param transform        A pointer to the transform
    void SetTransform(std::unique_ptr<Transform> transform);

    //! @brief Sets the material of the primitive
    //! @param material         A pointer to the shape
    void SetMaterial(const Material* material);

    //! @brief Returns the position of the current primitive
    //! @return                 The position of the primitive
    exrPoint3 GetPosition();

    //! @brief Returns the object to world transformation matrix of the primitive
    //! 
    //! Returns the object to world transformation matrix associated with the
    //! transform of the current primitive.
    //!
    //! @return                 The transformation matrix of the primitive
    Matrix4x4 GetObjectToWorldMatrix();

    //! @brief Returns the world to object transformation matrix of the primitive
    //! 
    //! Returns the world to object transformation matrix associated with the
    //! transform of the current primitive.
    //!
    //! @return                 The inverse matrix of the primitive
    Matrix4x4 GetWorldToObjectMatrix();

    //! Returns the material of the current primitive
    const Material* GetMaterial() const;
    
protected:
    //! The underlying shape that describes the primitive
    std::unique_ptr<Shape> m_Shape;

    //! The tranform of the primitive
    std::unique_ptr<Transform> m_Transform;

    //! The material assigned to the primitive (Owned by scene)
    const Material* m_Material;
};

exrEND_NAMESPACE