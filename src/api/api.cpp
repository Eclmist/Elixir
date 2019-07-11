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

#include "api.h"

#include "core/camera/camera.h"
#include "core/integrator/wittedintegrator.h"
#include "core/integrator/samplerintegrator.h"
#include "core/light/pointlight.h"
#include "core/material/diffuse.h"
#include "core/primitive/geometricprimitive.h"
#include "core/primitive/shape/box.h"
#include "core/primitive/shape/quad.h"
#include "core/primitive/shape/sphere.h"
#include "core/scene/scene.h"
#include "core/spatial/accelerator/accelerator.h"

exrBEGIN_NAMESPACE

struct RenderJob
{
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Scene> m_Scene;
    std::unique_ptr<Integrator> m_Integrator;
};

enum class APIState
{
    APISTATE_UNINITIALIZED,     // Before ElixirInit() or after ElixirCleanup(). No API calls are legal.
    APISTATE_OPTIONS,           // Scene-wide global options can be set
    APISTATE_SCENE,             // Scene may be described
    APISTATE_RENDERING          // Currently rendering scene. No API calls are legal.
};

static ElixirOptions g_RuntimeOptions;
static APIState g_CurrentAPIState = APIState::APISTATE_UNINITIALIZED;
static std::unique_ptr<RenderJob> g_CurrentRenderJob = nullptr;

void ElixirInit(const ElixirOptions& options)
{
    exrAssert(g_CurrentAPIState == APIState::APISTATE_UNINITIALIZED, "ElixirInit() has already been called!");

    g_RuntimeOptions = options;
    g_CurrentRenderJob = std::make_unique<RenderJob>();
    SampledSpectrum::Init();

    g_CurrentAPIState = APIState::APISTATE_OPTIONS;
}

void ElixirCleanup()
{
    exrAssert(g_CurrentAPIState != APIState::APISTATE_UNINITIALIZED, "ElixirCleanup() called before initialization!");
    g_CurrentAPIState = APIState::APISTATE_UNINITIALIZED;
}

void ElixirParseFile(const exrString& filename)
{
    if (filename == "-")
        ElixirSetupDemo();
    else
        exrError("Scene file parsing is not yet implemented!");
}

void ElixirSetupDemo()
{
    exrAssert(g_CurrentAPIState == APIState::APISTATE_OPTIONS, "ElixirSetupDemo() called before initialization!");
    exrPoint3 position(0.0f, 2.75f, 10.0f);
    exrPoint3 lookat(0.0f, 2.75f, 0.0f);
    exrFloat fov = 40.0f;
    exrFloat aspect = exrFloat(OutputWidth) / exrFloat(OutputHeight);
    exrFloat focusDist = (position - lookat).Magnitude();
    exrFloat aperture = 0.05f;
    g_CurrentRenderJob->m_Camera = std::make_unique<Camera>(position, lookat, exrVector3::Up(), fov, aspect, aperture, focusDist);
    g_CurrentRenderJob->m_Scene = std::make_unique<Scene>();

    // Scene objects to be added here.
    std::unique_ptr<GeometricPrimitive> geoPrimitive = std::make_unique<GeometricPrimitive>();
    Transform transform;
    transform.SetTranslation(exrVector3(0.0f, 2.75f, 0.0f));
    geoPrimitive->m_Shape = std::make_unique<Sphere>(transform, 1.0f);
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum(0.8f));
    std::unique_ptr<Primitive> p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // Back wall
    geoPrimitive = std::make_unique<GeometricPrimitive>();
    transform.SetTranslation(exrVector3(0.0f, 2.75f, -2.75f));
    geoPrimitive->m_Shape = std::make_unique<Quad>(transform, exrVector2(5.5f));
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum(1.0f));
    p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // left wall
    geoPrimitive = std::make_unique<GeometricPrimitive>();
    transform.SetTranslation(exrVector3(-2.75f, 2.75f, -0.0f));
    transform.SetRotation(exrVector3(0.0f, EXR_M_PIOVER2, 0.0f));
    geoPrimitive->m_Shape = std::make_unique<Quad>(transform, exrVector2(5.5f));
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum::FromRGB(exrVector3(1.0f, 0.0f, 0.0f), SpectrumType::Reflectance));
    p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // right wall
    geoPrimitive = std::make_unique<GeometricPrimitive>();
    transform.SetTranslation(exrVector3(2.75f, 2.75f, -0.0f));
    transform.SetRotation(exrVector3(0.0f, -EXR_M_PIOVER2, 0.0f));
    geoPrimitive->m_Shape = std::make_unique<Quad>(transform, exrVector2(5.5f));
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum::FromRGB(exrVector3(0.0f, 1.0f, 0.0f), SpectrumType::Reflectance));
    p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // ceiling
    geoPrimitive = std::make_unique<GeometricPrimitive>();
    transform.SetTranslation(exrVector3(0.0f, 5.5f, 0.0f));
    transform.SetRotation(exrVector3(EXR_M_PIOVER2, 0.0f, 0.0f));
    geoPrimitive->m_Shape = std::make_unique<Quad>(transform, exrVector2(5.5f));
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum(1.0f));
    p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // right wall
    geoPrimitive = std::make_unique<GeometricPrimitive>();
    transform.SetTranslation(exrVector3(0.0f, 0.0f, 0.0f));
    transform.SetRotation(exrVector3(-EXR_M_PIOVER2, 0.0f, 0.0f));
    geoPrimitive->m_Shape = std::make_unique<Quad>(transform, exrVector2(5.5f));
    geoPrimitive->m_Material = std::make_unique<Diffuse>(exrSpectrum(1.0f));
    p = std::move(geoPrimitive);
    g_CurrentRenderJob->m_Scene->AddPrimitive(p);

    // Lights
    transform.SetRotation(exrVector3::Zero());
    transform.SetTranslation(exrVector3(0.0f, 5.4f, 0.0f));
    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>(transform, 10.0f);
    std::unique_ptr<Light> l = std::move(pointLight);
    g_CurrentRenderJob->m_Scene->AddLight(l);

    // Init accel
    g_CurrentRenderJob->m_Scene->InitAccelerator();

    const exrU32 numSamples = 4;
    const exrU32 numBounces = 4;
    g_CurrentRenderJob->m_Integrator = std::make_unique<WittedIntegrator>(g_CurrentRenderJob->m_Camera, numBounces);
    g_CurrentAPIState = APIState::APISTATE_SCENE;
    // Setup scene..
}

void ElixirRender()
{
    exrAssert(g_CurrentAPIState == APIState::APISTATE_SCENE, "ElixirRendering() before scene has been described!");
    exrAssert(!g_CurrentRenderJob->m_Scene->m_SceneChanged, "Did we forget to call InitAccelerator()?");
    g_CurrentAPIState = APIState::APISTATE_RENDERING;

    // Do render/write file
    g_CurrentRenderJob->m_Integrator->Render(*g_CurrentRenderJob->m_Scene);
}
exrEND_NAMESPACE
