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

#include "surfaceinteraction.h"
#include "core/primitive/primitive.h"
#include "core/ray/raydifferential.h"

// todo: fix memory leak hack 
#include "core/bsdf/bsdf.h"


exrBEGIN_NAMESPACE

SurfaceInteraction::SurfaceInteraction(const exrPoint3& point, const exrVector3& wo,
    const exrVector3& normal, const Shape* shape)
    : Interaction(point, normal, wo)
    , m_Shape(shape)
    , m_BSDF(nullptr) {};


SurfaceInteraction::~SurfaceInteraction()
{
    delete m_BSDF;
}

void SurfaceInteraction::ComputeScatteringFunctions(const RayDifferential& ray)
{
    m_Primitive->ComputeScatteringFunctions(this);
}

exrSpectrum SurfaceInteraction::EvaluateEmission(const exrVector3& w) const
{
    const AreaLight* arealight = m_Primitive->GetAreaLight();
    return arealight ? arealight->Sample(*this, w) : exrSpectrum(0.0f);
}

exrEND_NAMESPACE