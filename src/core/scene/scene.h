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
#include "core/light/light.h"
#include "core/primitive/primitive.h"
#include "core/spatial/accelerator/accelerator.h"

exrBEGIN_NAMESPACE

//! @brief A scene object that owns a collection of primitives
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
    //! @param primitive        A pointer to the primitive
    void AddPrimitive(std::unique_ptr<Primitive> primitive);

    //! @brief Adds a light to the scene
    //! 
    //! This function adds a light to the scene's light collection
    //! Lights will be importance sampled
    //! 
    //! @param light            A pointer to the light
    void AddLight(std::shared_ptr<Light> light);

    //! @brief Initializes the scene's accelerator if it has yet to be initialized or needs to be updated
    void InitAccelerator();

public:
    //! @brief Returns the number of primitive in the scene
    //! @return                 The number of primitives in the scene
    inline exrU64 GetSceneSize() const { return static_cast<exrU64>(m_Primitives.size()); }

public:
    //! A collection of pointers that points to primitives in the scene
    std::vector<std::unique_ptr<Primitive>> m_Primitives;

    //! A collection of pointers that points to lights in the scene
    //! Shared ptr because lights could be either owned by the scene or by primitives
    std::vector<std::shared_ptr<Light>> m_Lights;
private:
    //! The accelerator to use
    std::unique_ptr<Accelerator> m_Accelerator;

    //! A flag that determines if the scene has changed since the last render
    exrBool m_IsDirty;

    //! The bounding volume that completely encapsulates the scene
    AABB m_WorldBound;
};

exrEND_NAMESPACE