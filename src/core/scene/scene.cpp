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
#include "core/spatial/accelerator/bvh.h"
#include "core/light/arealight.h"

exrBEGIN_NAMESPACE

Scene::Scene(Accelerator::AcceleratorType accelType)
    : m_AcceleratorType(accelType) { }

Scene::Scene(std::vector<std::unique_ptr<Primitive>>& primitives,std::vector<std::unique_ptr<Light>>& lights,
    Accelerator::AcceleratorType accelType)
    : m_AcceleratorType(accelType)
{
    for (exrU32 i = 0; i < primitives.size(); ++i)
        m_Primitives.push_back(std::move(primitives[i]));

    for (exrU32 i = 0; i < lights.size(); ++i)
        m_SceneLights.push_back(std::move(lights[i]));
}

void Scene::AddPrimitive(std::unique_ptr<Primitive>& primitive)
{
    // If primitive is an area light, add it to the lights collection
    AreaLight* primitiveLight = primitive->GetAreaLight();
    if (primitiveLight) 
        AddLight(*primitiveLight);

    m_Primitives.push_back(std::move(primitive));
    m_SceneChanged = true;
}

void Scene::AddLight(std::unique_ptr<Light>& light)
{
    light->Preprocess(*this);
    m_Lights.push_back(light.get());
    m_SceneLights.push_back(std::move(light));
}

void Scene::AddLight(Light& light)
{
    light.Preprocess(*this);
    m_Lights.push_back(&light);
}

void Scene::InitAccelerator()
{
    m_SceneChanged = false;

    std::vector<Primitive*> primitivePtrs;

    // shallow copy pointer values to be used by bvh accel
    for (exrU32 i = 0; i < m_Primitives.size(); ++i)
        primitivePtrs.push_back(m_Primitives[i].get());

    switch (m_AcceleratorType)
    {
    case Accelerator::ACCELERATORTYPE_BVH:
        m_Accelerator = std::make_unique<BVHAccelerator>(primitivePtrs);
        break;
    case Accelerator::ACCELERATORTYPE_KDTREE:
        throw "KDTree is not yet implemented!";
    default:
        throw "Invalid accelerator type!";
    }
}

exrBool Scene::Intersect(const Ray& ray, SurfaceInteraction* interaction) const
{
    exrAssert(m_Accelerator, "Scene accelerator has not yet been initialized!");
    return m_Accelerator->Intersect(ray, interaction);
}

exrBool Scene::HasIntersect(const Ray& ray) const
{
    exrAssert(m_Accelerator, "Scene accelerator has not yet been initialized!");
    return m_Accelerator->HasIntersect(ray);
}

exrSpectrum Scene::SampleSkyLight(const Ray& ray) const
{
    // Hard coded values for now
    exrVector3 sunsetRed(0.725f, 0.268f, 0.152f);
    exrVector3 sunsetBlue(0.18f, 0.296f, 0.952f);
    exrVector3 direction = ray.m_Direction.Normalized();
    exrFloat t = (direction.y + 0.5f) / 1.2f;
    return exrSpectrum::FromRGB(exrLerp(sunsetRed, sunsetBlue, exrSaturate(t)));
}


exrEND_NAMESPACE