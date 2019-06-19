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
#include "camera.h"
#include "scene.h"

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
    // Integrator
    // Scene
    Scene* CreateScene();
    Camera* CreateCamera();
    // Camera (type + transform)
    // Accelerator

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

    exrError("Scene file parsing is not yet implemented!");
}

void ElixirSetupDemo()
{
    CurrentRenderJob = std::make_unique<RenderJob>();
    CurrentRenderJob->
}

void ElixirRender()
{
    
}
exrEND_NAMESPACE
