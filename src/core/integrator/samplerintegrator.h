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

#include "integrator.h"
#include "core/camera/camera.h"

exrBEGIN_NAMESPACE

constexpr exrU32 TileSize = 16;

class SamplerIntegrator : public Integrator
{
public:
    SamplerIntegrator(Camera* camera, exrU32 numSamplesPerPixel, exrU32 numBouncePerPixel)
        : m_Camera(camera)
        , m_NumSamplesPerPixel(numSamplesPerPixel)
        , m_NumBouncePerPixel(numBouncePerPixel) {};

    virtual exrSpectrum Li(const Ray& ray, const Scene& scene, MemoryArena& arena, exrU32 depth = 0) const = 0;

    void Render(const Scene& scene) override;

protected:
    virtual exrSpectrum SpecularReflect(const Ray& ray, const SurfaceInteraction& intersect,
        const Scene& scene, MemoryArena& arena, exrU32 depth) const;

    virtual exrSpectrum SpecularRefract(const Ray& ray, const SurfaceInteraction& intersect,
        const Scene& scene, MemoryArena& arena, exrU32 depth) const;

    exrSpectrum UniformSampleOneLight(const Interaction& it, const Scene& scene,
        MemoryArena& arena) const;


protected:
    Camera* m_Camera;
    exrU32 m_NumSamplesPerPixel;
    exrU32 m_NumBouncePerPixel;
};

exrEND_NAMESPACE