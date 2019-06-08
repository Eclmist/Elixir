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

exrBEGIN_NAMESPACE

/* ==========================================================================
    Global API Settings
*/
Options ElixirRuntimeOptions;


/* ==========================================================================
    API Static Data
*/
enum class APIState
{
    APISTATE_UNINITIALIZED,     // Before ElixirInit() or after ElixirCleanup(). No API calls are legal.
    APISTATE_CONFIGURATION,     // Scene-wide global options can be set
    APISTATE_SCENESETUP,        // Scene may be described
};

static APIState CurrentAPIState = APIState::APISTATE_UNINITIALIZED;


/* ==========================================================================
    API Macros
*/
#define VERIFY_INITIALIZED(func)                                            \
    if (CurrentAPIState == APIState::APISTATE_UNINITIALIZED) {              \
        exrError("ElixirInit() must be called before " << func << "()!");   \
        return;                                                             \
    }                                                                       \

/* ==========================================================================
    API Function definitions
*/
void ElixirInit(const Options& options)
{
    ElixirRuntimeOptions = options;

    if (CurrentAPIState != APIState::APISTATE_UNINITIALIZED)
        exrError("ElixirInit() has already been called!");

    CurrentAPIState = APIState::APISTATE_CONFIGURATION;
}

void ElixirCleanup()
{
    VERIFY_INITIALIZED("ElixirCleanup");
    CurrentAPIState = APIState::APISTATE_UNINITIALIZED;
}

void ElixirParseFile(const exrString& filename)
{
    if (filename == "-")

    else
        exrError("Scene file parsing is not yet implemented!");
}

exrEND_NAMESPACE
