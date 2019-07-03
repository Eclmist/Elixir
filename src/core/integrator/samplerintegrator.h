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

static constexpr exrU32 TileSize = 16;

class SamplerIntegrator : public Integrator
{
public:
    SamplerIntegrator(std::unique_ptr<Camera>& camera, exrU32 numSamplesPerPixel, exrU32 numBouncePerPixel)
        : m_Camera(std::move(camera))
        , m_NumSamplesPerPixel(numSamplesPerPixel)
        , m_NumBouncePerPixel(numBouncePerPixel) {};

    virtual void Preprocess(const Scene& scene) {};
    virtual exrSpectrum Reflect(const RayDifferential& ray, const SurfaceInteraction& intersect, const Scene& scene, exrU32 depth) const;
    virtual exrSpectrum Refract(const RayDifferential& ray, const SurfaceInteraction& intersect, const Scene& scene, exrU32 depth) const;
    virtual exrSpectrum Evaluate(const RayDifferential& ray, const Scene& scene, exrU32 depth = 0) const = 0;

    void Render(const Scene& scene) override;

protected:
    std::unique_ptr<Camera> m_Camera;
    exrU32 m_NumSamplesPerPixel;
    exrU32 m_NumBouncePerPixel;
};

exrEND_NAMESPACE