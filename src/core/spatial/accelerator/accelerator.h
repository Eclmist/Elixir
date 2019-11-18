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

//! @brief Defines the base class for hierarchical and spacial accelerators
class Accelerator
{
public:

    enum AcceleratorType
    {
        ACCELERATORTYPE_BVH,
        ACCELERATORTYPE_KDTREE // Not implemented (maybe no point? BVH is much faster.)
    };

    //! @brief Test the accelerator for intersections with a ray
    //! 
    //! Test all accelerator nodes for intersections with ray, and output a
    //! smaller set of primitives that the caller can manually test intersections
    //! with.
    //! 
    //! @param ray              The ray to test against.
    //! @return                 The collection of objects that the ray could possibly
    //!                         intersect with.
    virtual std::vector<Primitive*> Intersect(const Ray& ray) const = 0;

    //! @brief Test every primitive in the accelerator for intersection with input ray
    //! 
    //! Test every primitive for ray intersection, and output true if the ray passes
    //! intersection tests with at least one primitive in the accelerator.
    //! 
    //! @param ray              The ray to test against
    //! @return                 True if the there is an intersection
    virtual exrBool HasIntersect(const Ray& ray) const = 0;
};

exrEND_NAMESPACE