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
#include "camera/camera.h"
#include "scene/scene.h"

exrBEGIN_NAMESPACE

/* ==========================================================================
    Global API Settings
*/
ElixirOptions ElixirRuntimeOptions;

/* ==========================================================================
    API Local Classes
*/
struct RenderJob
{
    Scene* MakeScene();
    Camera* MakeCamera();

    exrString m_SamplerName = "halton";
    exrString m_AcceleratorName = "bvh";
    exrString m_IntegratorName = "pathtracer";
    exrString m_CameraName = "perspective";
    
    std::vector<std::unique_ptr<Light>> m_Lights;
    std::vector<std::unique_ptr<Primitive>> m_Primitives;
};

/* ==========================================================================
    API Static Data
*/
enum class APIState
{
    APISTATE_UNINITIALIZED,     // Before ElixirInit() or after ElixirCleanup(). No API calls are legal.
    APISTATE_SETUP_OPTION,      // Scene-wide global options can be set
    APISTATE_SETUP_SCENE,       // Scene may be described
    APISTATE_RENDERING          // Currently rendering scene. No API calls are legal.
};

static APIState CurrentAPIState = APIState::APISTATE_UNINITIALIZED;
static std::unique_ptr<RenderJob> CurrentRenderJob = nullptr;

/* ==========================================================================
    API Macros
*/
#define VERIFY_UNINITIALIZED(func)\
    if (CurrentAPIState != APIState::APISTATE_UNINITIALIZED) {\
        exrError(func << "() must be called before ElixirInit()!");\
        return;\
    }\

#define VERIFY_INITIALIZED(func)\
    if (CurrentAPIState == APIState::APISTATE_UNINITIALIZED) {\
        exrError("ElixirInit() must be called before " << func << "()!");\
        return;\
    }\

/* ==========================================================================
    API Function definitions
*/
void ElixirInit(const ElixirOptions& options)
{
    if (CurrentAPIState != APIState::APISTATE_UNINITIALIZED)
        exrError("ElixirInit() has already been called!");

    ElixirRuntimeOptions = options;
    SampledSpectrum.Init();

    CurrentAPIState = APIState::APISTATE_SETUP_OPTION;
}

void ElixirCleanup()
{
    VERIFY_INITIALIZED("ElixirCleanup");
    CurrentAPIState = APIState::APISTATE_UNINITIALIZED;
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
    // light
    CurrentRenderJob.m_Lights->push_back(std::make_unique<Quad>(exrPoint3(0.0f, 5.5f, 0.0f), exrVector2(1.3f, 1.0f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<DiffuseLight>(exrVector3(1.0f, 0.77f, 0.4f) * 7.0f)));

    // room
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Quad>(exrPoint3(-2.75f, 2.75f, 0.0f), exrVector2(5.6f, 5.5f), exrVector3(0, exrDegToRad(90), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 0.0f, 0.0f))));
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Quad>(exrPoint3(2.75f, 2.75f, 0.0f), exrVector2(5.6f, 5.5f), exrVector3(0, exrDegToRad(-90.0f), 0), std::make_unique<Lambertian>(exrVector3(0.0f, 1.0f, 0.0f))));
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Quad>(exrPoint3(0.0f, 2.75f, -2.80f), exrVector2(5.5f), exrVector3(0.0f), std::make_unique<Lambertian>(exrVector3(1.0f))));
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Quad>(exrPoint3(0.0f, 0.0f, 0.0f), exrVector2(5.5f, 5.6f), exrVector3(exrDegToRad(-90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Quad>(exrPoint3(0.0f, 5.5f, 0.0f), exrVector2(5.5f, 5.6f), exrVector3(exrDegToRad(90), 0, 0), std::make_unique<Lambertian>(exrVector3(1.0f))));

    // objects
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Box>(exrPoint3(-0.9f, 1.8f, -1.0f), exrVector3(1.6f, 3.6f, 1.6f), exrVector3(0, exrDegToRad(110), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 1.0f, 1.0f))));
    CurrentRenderJob.m_Primitives->push_back(std::make_unique<Box>(exrPoint3(0.9f, 0.8f, 1.0f), exrVector3(1.6f, 1.6f, 1.6f), exrVector3(0, exrDegToRad(-20), 0), std::make_unique<Lambertian>(exrVector3(1.0f, 1.0f, 1.0f))));
}

void ElixirRender()
{
    
}
exrEND_NAMESPACE
