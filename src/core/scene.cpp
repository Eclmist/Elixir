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
#include "accelerator/bvh.h"

exrBEGIN_NAMESPACE

void Scene::AddShape(std::unique_ptr<Shape> shape) 
{
    m_Shapes.push_back(std::move(shape));
    m_IsDirty = true;
}

void Scene::AddEmissiveShape(std::unique_ptr<Shape> emissiveShape)
{
    m_EmissiveShapes.push_back(std::move(emissiveShape));
    m_IsDirty = true;
}

exrBool Scene::RaytraceScene(const Ray& ray, exrFloat tMin, exrFloat tMax, Interaction& hitInfo) const
{
    return m_Accelerator->Intersect(ray, tMin, tMax, hitInfo);
}

void Scene::InitializeBvh()
{
    if (m_Accelerator == nullptr || m_IsDirty)
    {
        exrProfile("Building BVH Tree")

        std::vector<Shape*> shapePtrs;

        // shallow copy pointer values to be used by bvh accel
        for (exrU32 i = 0; i < m_Shapes.size(); i++)
        {
            shapePtrs.push_back(m_Shapes[i].get());
        }

        for (exrU32 i = 0; i < m_EmissiveShapes.size(); i++)
        {
            shapePtrs.push_back(m_EmissiveShapes[i].get());
        }

        m_Accelerator = std::make_unique<BVHAccelerator>(shapePtrs);
        m_IsDirty = false;
        exrEndProfile()
    }
}

exrEND_NAMESPACE
