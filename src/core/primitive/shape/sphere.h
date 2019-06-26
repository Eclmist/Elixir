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
    Sphere(const Transform& transform, exrFloat radius)
        : Shape(transform, ComputeBoundingVolume())
        , m_Radius(radius) {};

    //! @brief Test the sphere for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray in the domain
    //! of tMin and tMax
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const override;

    //! @brief Test the sphere for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, but
    //! does not initialize interaction or hit info. Useful for when checking if a ray
    //! is obstructed, such as with shadow rays
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! 
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const override;

    //! @brief Samples a point on the surface of the sphere
    //!
    //! Chooses a point on the surface of the sphere using a sampling distribution with respect
    //! to the surface area of the sphere and returns the local geometric information about the
    //! sampled point in an Interaction
    //!
    //! @param u                I don't know what this does or why PBRT uses this, fuck it i'm leaving it here because maybe years later I would look back and go "ohhhhhh" that's what "u" was used for. But in the meantime I'm just gonna ignore all that and do it the naive way.
    //!
    //! @return                 The interaction at the sampled point
    Interaction Sample(const exrPoint2& u) const override;

    //! @brief Returns the total surface area of the sphere
    //!
    //! The total surface area of the sphere. This can be used in constructing PDF and other
    //! calculations
    //!
    //! @return                 The total surface area of the sphere
    exrFloat GetArea() const override;

    //! @brief Computes a bounding volume
    //! 
    //! Computes the a bounding volume that encapsulates the current sphere.
    //! 
    //! @return                 Always return true since bounding volumes can be created for spheres
    AABB ComputeBoundingVolume() override;

private:
    //! The radius of the sphere
    exrFloat m_Radius;

};

exrEND_NAMESPACE