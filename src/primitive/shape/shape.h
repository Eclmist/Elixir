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
#include "primitive/transform.h"
#include "spatial/aabb.h"
#include "material/material.h"

exrBEGIN_NAMESPACE

//! @brief A base class that all shapes should inherit from
//! 
//! A base class that all shape shapes such as triangles and spheres should inherit from.
//! Contains various pure virtual functions that should be overridden by individual shapes,
//! such as for computing its bounding volume and handling ray intersections
class Shape
{
public:
    //! Constructor
    Shape(const Transform& transform, const AABB& boundingVolume)
        : m_Transform(transform)
        , m_BoundingVolume(boundingVolume) {};

public:
    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, and
    //! outputs the interaction info into <interaction>
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool Intersect(const Ray& ray, exrFloat& tHit, SurfaceInteraction* interaction) const = 0;

    //! @brief Test the geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, but
    //! does not initialize interaction or hit info. Useful for when checking if a ray
    //! is obstructed, such as with shadow rays
    //! 
    //! @param ray              The ray to test against
    //! @param tHit             The t value of ray at the point of intersection, if any
    //! 
    //! @return                 True if the there is an intersection
    virtual exrBool HasIntersect(const Ray& ray, exrFloat& tHit) const = 0;

    //! @brief Samples a point on the surface of the shape
    //!
    //! Chooses a point on the surface of the shape using a sampling distribution with respect
    //! to the surface area of the shape and returns the local geometric information about the
    //! sampled point in an Interaction
    //!
    //! @param u                Uniformly sampled 2D point
    //!
    //! @return                 The interaction at the sampled point
    virtual Interaction Sample(const exrPoint2& u) const = 0;

    //! @brief Samples a point with respect to the solid angle from reference point ref
    //!
    //! Samples a point on the surface of the shape by taking a point from which the surface of
    //! the shape is being integrated over as a parameter. The sample point is picked using a
    //! density with respect to the solid angle from the reference point ref.
    //!
    //! @param ref              The current point of the integrand
    //! @param u                Uniformly sampled 2D point
    //!
    //! @return                 The interaction at the sampled point
    virtual Interaction Sample(const Interaction& ref, const exrPoint2& u) const { return Sample(u); }

    //! @brief Samples the PDF of the current shape with the interaction
    //!
    //! Returns the probability according to the probability density with respect to the surface
    //! area of the shape, for the current interaction. Shapes almost always sample uniformly by
    //! area on their surface, and therefore the default implementation provided simply returns
    //! one over the surface area of the shape.
    //!
    //! @return                 The probability of the current interaction
    virtual exrFloat SamplePDF(const Interaction& interaction) const { return 1 / GetArea(); }

    //! @brief Samples the PDF over the solid angle with regards to reference point ref
    //!
    //! Returns the probability according to the probability density with respect to
    //! the solid angle of the shape, projected towards reference point ref.
    //!
    //! @return                 The probability of the current interaction
    //virtual exrFloat SamplePDF(const Interaction& ref, const exrVector3& wi) const;
    
    //! @brief Returns the total surface area of the shape
    //!
    //! The total surface area of the shape. This can be used in constructing PDF and other
    //! calculations
    //!
    //! @return                 The total surface area of the sphere
    virtual exrFloat GetArea() const = 0;

    //! @brief Returns the solid angle of the shape projected on unit sphere around point p
    //!
    //! Returns the solid angle subtended by the shape with regards to the reference point
    //! p, given in world space. Some shapes compute this value in closed-form, while the
    //! default uses Monte Carlo integration
    //!
    //! @param p                The point of reference
    //! @param nbSamples        The number of samples to use if computing with MC
    //!
    //! @return                 The solid angle subtended by the shape w.r.t point p
    virtual exrFloat SolidAngle(const exrPoint3& p, exrU32 nbSamples = 512) const;

public:
    //! @brief Returns the bounding volume of the shape
    //! @return                 A bounding volume of the shape
    inline AABB GetBoundingVolume()
    {
        return m_BoundingVolume;
    };

protected:
    //! Computes a bounding volume that encapsulates the current geometry.
    virtual AABB ComputeBoundingVolume() = 0;

protected:
    //! A bounding volume that contains the shape;
    const AABB m_BoundingVolume;

    //! The transform of the shape
    const Transform m_Transform;
};

exrEND_NAMESPACE