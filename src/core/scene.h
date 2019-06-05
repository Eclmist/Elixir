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
#include "shape/shape.h"

class Accelerator;
struct Interaction;

exrBEGIN_NAMESPACE

//! @brief A scene object that owns a collection of shapes
//! 
//! A scene class that contains a collection of shapes and various helper functions
//! to interact with the collection.
class Scene
{
public:
    //! @brief Adds a shape to the scene
    //! 
    //! This function adds a new shape to the scene's shape collection
    //! 
    //! @param shape           A pointer to the shape
    void AddShape(std::unique_ptr<Shape> shape);

    //! @brief Adds a emissive shape to the scene
    //! 
    //! This function adds a emissive shape to the scene's shape collection
    //! Emissive shapes will be importance sampled
    //! 
    //! @param shape           A pointer to the shape
    void AddEmissiveShape(std::unique_ptr<Shape> emissiveShape);

    //! @brief Raytrace through the scene and returns the info of the nearest interaction point
    //! 
    //! This function and executes an intersection test with the scene objects
    //! with the input ray.
    //! 
    //! @param ray              The ray to test against
    //! @param tMin             Min t value of ray to test
    //! @param tMax             Max t value of ray to test
    //! @param interaction      Output struct that contains the interaction information of the nearest interaction point
    //! 
    //! @return                 True if the there is at least one intersection
    exrBool RaytraceScene(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& interaction) const;

    //! @brief Initializes the scene BVH if it has yet to be initialized or needs to be updated
    void InitializeBvh();

public:
    //! @brief Returns the number of shape in the scene
    //! @return                 The number of shapes in the scene
    inline exrU64 GetSceneSize() const { return static_cast<exrU64>(m_Shapes.size() + m_EmissiveShapes.size()); }

private:
    //! A collection of pointers that points to shapes in the scene
    std::vector<std::unique_ptr<Shape>> m_Shapes;

    //! A collection of pointers that points to emissive shapes in the scene
    std::vector<std::unique_ptr<Shape>> m_EmissiveShapes;

    //! The accelerator to use
    std::unique_ptr<Accelerator> m_Accelerator;

    //! A flag that determines if the scene has changed since the last render
    exrBool m_IsDirty;
};

exrEND_NAMESPACE