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

#include "scene.h"
#include "spatial/accelerator/bvh.h"
#include "light/arealight.h"

exrBEGIN_NAMESPACE

void Scene::AddPrimitive(std::unique_ptr<Primitive> primitive)
{
    m_Primitives.push_back(std::move(primitive));

    // If primitive is an area light, add it to the lights collection
    const AreaLight* primitiveLight = primitive->GetAreaLight();
    if (primitiveLight) 
        AddLight(std::make_shared<AreaLight>(primitiveLight));

    m_IsDirty = true;
}

void Scene::AddLight(std::shared_ptr<Light> light)
{
    m_Lights.push_back(light);
    m_IsDirty = true;
}

void Scene::InitAccelerator()
{
    if (m_Accelerator == nullptr || m_IsDirty)
    {
        exrProfile("Building BVH Tree")

        std::vector<Primitive*> primitivePtrs;

        // shallow copy pointer values to be used by bvh accel
        for (exrU32 i = 0; i < m_Primitives.size(); i++)
        {
            primitivePtrs.push_back(m_Primitives[i].get());
        }

        m_Accelerator = std::make_unique<BVHAccelerator>(primitivePtrs);
        m_IsDirty = false;
        exrEndProfile()
    }
}

exrEND_NAMESPACE