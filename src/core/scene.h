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
#include "accelerator/accelerator.h"
#include "geometry/primitive.h"

class Accelerator;
struct PrimitiveHitInfo;

exrBEGIN_NAMESPACE

//! @brief A scene object that owns primitives
//! 
//! A scene class that contains a collection of primitives and various helper functions
//! to interact with the collection.
class Scene
{
public:
    //! @brief Adds a primitive to the scene
    //! 
    //! This function adds a new primitive to the scene's primitive collection
    //! 
    //! @param primitive         A pointer to the primitive
    void AddPrimitive(std::unique_ptr<Primitive> primitive);

    //! @brief Raytrace through the scene and returns the info of the nearest hit point
    //! 
    //! This function and executes an intersection test with the scene objects
    //! with the input ray.
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param hitInfo          Output struct that contains the hit information of the nearest hit point
    //! 
    //! @return                 True if the there is at least one intersection
    exrBool RaytraceScene(const Ray& ray, exrFloat tMin, exrFloat tMax, PrimitiveHitInfo& hitInfo) const;

    //! @brief Initializes the scene BVH if it has yet to be initialized or needs to be updated
    void InitializeBvh();

public:
    //! @brief Returns the number of primitives in the scene
    //! @return                 The number of primitives in the scene
    inline exrU64 GetSceneSize() const { return static_cast<exrU64>(m_Primitives.size()); }

private:
    //! A collection of pointers that points to primitives in the scene
    std::vector<std::unique_ptr<Primitive>> m_Primitives;

    //! The accelerator to use
    std::unique_ptr<Accelerator> m_Accelerator;

    //! A flag that determines if the scene has changed since the last render
    exrBool m_IsDirty;
};

exrEND_NAMESPACE