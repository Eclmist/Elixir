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

    Scene(Accelerator::AcceleratorType accelType = Accelerator::AcceleratorType::ACCELERATORTYPE_BVH);

    Scene(std::vector<std::unique_ptr<Primitive>>& primitives, std::vector<std::unique_ptr<Light>>& lights,
        Accelerator::AcceleratorType accelType);

    //! @brief Adds a primitive to the scene
    //! 
    //! This function adds a new primitive to the scene's primitive collection
    //! 
    //! @param primitive        A pointer to the primitive
    void AddPrimitive(std::unique_ptr<Primitive>& primitive);

    //! @brief Adds a light to the scene
    //! 
    //! This function adds a light to the scene's light collection
    //! Lights will be importance sampled
    //! 
    //! @param light            A pointer to the light
    void AddLight(std::unique_ptr<Light>& light);

    //! @brief Initializes the scene's accelerator if it has yet to be initialized or needs to be updated
    void InitAccelerator();

    //! @brief Returns the number of primitive in the scene
    //! @return                 The number of primitives in the scene
    inline exrU64 GetSceneSize() const { return static_cast<exrU64>(m_Primitives.size()); };

    //! @brief Test the accelerator for intersections with a ray
    //! 
    //! Test all geometry for intersection with the ray, and outputs the surface
    //! intersection data in <interaction> 
    //! 
    //! @param ray              The ray to test against
    //! @param interaction      Output struct that contains the interaction information
    //! 
    //! @return                 True if the there are any intersections
    exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const;

    //! @brief Test the scene geometry for intersections with a ray
    //! 
    //! This function allows us to do intersection tests with a segment of a ray, but
    //! does not initialize interaction or hit info. Useful for when checking if a ray
    //! is obstructed, such as with shadow rays
    //! 
    //! @param ray              The ray to test against
    //! 
    //! @return                 True if the there is an intersection
    exrBool HasIntersect(const Ray& ray) const;
    
    //! @brief Return the skylight color for the given ray direction
    //!
    //! @param ray              The reference ray
    //!
    //! @return                 The skylight color
    exrSpectrum SampleSkyLight(const Ray& ray) const;

public:
    //! A collection of pointers that points to lights in the scene
    //! ALL lights, including those owned by primitives
    std::vector<Light*> m_Lights;

    //! A flag that can remind us to re-init the accelerator after scene updates
    exrBool m_SceneChanged = true;

private:
    void AddLight(Light& light);

private:
    //! The type of accelerator to use
    Accelerator::AcceleratorType m_AcceleratorType;

    //! The accelerator to use
    std::unique_ptr<Accelerator> m_Accelerator;

    //! Lights that the scene owns and doesn't belong to any primitive.
    std::vector<std::unique_ptr<Light>> m_SceneLights;

    //! A collection of pointers that points to primitives in the scene
    std::vector<std::unique_ptr<Primitive>> m_Primitives;
};

exrEND_NAMESPACE