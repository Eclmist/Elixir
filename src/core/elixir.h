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

#include "system/system.h"
#include "system/profiling/profiler.h"

#include "math/math.h"
#include "math/conversionutils.h"

#include "core/interaction/surfaceinteraction.h"
#include "core/sampling/random.h"
#include "core/spectrum/sampledspectrum.h"
#include "core/spectrum/rgbspectrum.h"
#include "core/ray/raydifferential.h"

exrBEGIN_NAMESPACE

static constexpr exrU32 OutputWidth = 500;
static constexpr exrU32 OutputHeight = 500;

struct ElixirOptions
{
    exrU32          numThreads;
    exrString       outputFile;
    exrBool         quickRender;
    exrBool         quiet;
    exrBool         debug;
};

exrEND_NAMESPACE
