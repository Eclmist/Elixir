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

#include <tsl_version.h>
#include <tsl_args.h>
#include <tsl_system.h>
#include <tsl_define.h>

#include "core/elixir.h"
#include "core/material/material.h"

exrBEGIN_NAMESPACE

class BxDF;
class Material;

// Initialize tiny shading language system
// It basically takes the chances to initialize all necessary data structure, like registering callback,
// closure type, tsl global data structure.
// This is considered the set up of TSL system, it is totally fine to leave it in one single thread, since
// it is fairly cheap to do so, multi-threading the initialization literally means nothing.
void initialize_tsl_system();
void initialize_tsl_material(Material* matte);

BxDF* GetBxDF(const Material* const material, const SurfaceInteraction& si, MemoryArena& arena);

exrEND_NAMESPACE