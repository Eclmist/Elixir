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

#include "samplerintegrator.h"

exrBEGIN_NAMESPACE

class PathTracer : public SamplerIntegrator
{
public:
    PathTracer(Camera* camera, exrU32 numSamplesPerPixel, exrU32 numBouncePerPixel)
        : SamplerIntegrator(camera, numSamplesPerPixel, numBouncePerPixel) {};

    exrSpectrum Li(const Ray& ray, const Scene& scene, MemoryArena& arena, exrU32 depth = 0) const override;
};

exrEND_NAMESPACE