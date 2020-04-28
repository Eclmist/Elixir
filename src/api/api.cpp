/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

#include "api.h"

#include "core/camera/camera.h"
#include "core/integrator/pathintegrator.h"
#include "core/light/pointlight.h"
#include "core/light/directionallight.h"
#include "core/material/dielectric.h"
#include "core/material/matte.h"
#include "core/material/metal.h"
#include "core/primitive/mesh.h"
#include "core/primitive/primitive.h"
#include "core/primitive/shape/quad.h"
#include "core/primitive/shape/sphere.h"
#include "core/primitive/shape/triangle.h"
#include "core/scene/scene.h"
#include "core/spatial/accelerator/accelerator.h"

exrBEGIN_NAMESPACE

ElixirOptions g_RuntimeOptions;

// These options PER RENDER options. Scenes, cameras, integrators, etc.
// General elixir settings (number of threads, etc) should go into ElixirOptions.
struct RenderJob
{
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Scene> m_Scene;
    std::unique_ptr<Integrator> m_Integrator;
};

static std::unique_ptr<RenderJob> g_CurrentRenderJob = nullptr;

void ElixirInit(const ElixirOptions& options)
{
    g_RuntimeOptions = options;
    g_CurrentRenderJob = std::make_unique<RenderJob>();
}

void ElixirCleanup()
{
    // Nothing to clean up for now.
}

void ElixirParseFile(const exrString& filename)
{
    // Load obj file
    exrPoint3 position(2.0f, 3.75f, 10.0f);
    exrPoint3 lookat(-0.6f, 2.0f, 0.0f);
    exrFloat fov = 40.0f;
    exrFloat focusDist = (position - lookat).Magnitude();
    exrFloat aperture = 1 / 20.0f;
    g_CurrentRenderJob->m_Camera = std::make_unique<Camera>(position, lookat, exrVector3::Up(), fov, aperture, focusDist);
    g_CurrentRenderJob->m_Scene = std::make_unique<Scene>();

    // Setup materials in the scene
    // 0 - White
    g_CurrentRenderJob->m_Scene->AddMaterial(std::make_unique<Matte>(exrSpectrum::FromRGB(exrVector3(1.0f, 1.0f, 1.0f)), 20.0f));
    // 1 - Red
    g_CurrentRenderJob->m_Scene->AddMaterial(std::make_unique<Matte>(exrSpectrum::FromRGB(exrVector3(1.0f, 0.0f, 0.0f)), 20.0f));
    // 2 - Green
    g_CurrentRenderJob->m_Scene->AddMaterial(std::make_unique<Matte>(exrSpectrum::FromRGB(exrVector3(0.0f, 1.0f, 0.0f)), 20.0f));
    // 3 - Glossy
    g_CurrentRenderJob->m_Scene->AddMaterial(std::make_unique<Metal>(exrSpectrum::FromRGB(exrVector3(1.022f, 0.782f, 0.344f))));

    // Setup scene primitives
    // Sphere
    std::unique_ptr<Primitive> primitive = std::make_unique<Primitive>();
    Transform transform;

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh::LoadFromFile(filename.c_str()));

    for (exrU32 i = 0; i < mesh->m_NumFaces; ++i)
    {
        primitive = std::make_unique<Primitive>();
        primitive->SetShape(std::make_unique<Triangle>(mesh, i));
        primitive->SetMaterial(g_CurrentRenderJob->m_Scene->GetMaterial(0));
        g_CurrentRenderJob->m_Scene->AddPrimitive(std::move(primitive));
    }

    transform.SetTranslation(exrVector3(0,100,0));
    transform.SetRotation(exrVector3(exrDegToRad(0), exrDegToRad(0), exrDegToRad(40)));
    g_CurrentRenderJob->m_Scene->AddLight(std::make_unique<DirectionalLight>(transform, exrSpectrum::FromRGB(exrVector3(1,0.7,0.2)) * 2));

    // Init accel
    g_CurrentRenderJob->m_Scene->InitAccelerator();

    const exrU32 numSamples = 8;
    const exrU32 numBounces = 8;
    g_CurrentRenderJob->m_Integrator = std::make_unique<PathIntegrator>(g_CurrentRenderJob->m_Camera.get(), numSamples, numBounces);
}

void ElixirRender()
{
    // Do render/write file
    g_CurrentRenderJob->m_Integrator->Render(*g_CurrentRenderJob->m_Scene);
}

exrEND_NAMESPACE
