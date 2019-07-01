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

exrBEGIN_NAMESPACE

SurfaceInteraction::SurfaceInteraction(const exrPoint3& point, const exrVector3& wo,
    const exrVector3& dpdu, const exrVector3& dpdv,
    const exrVector3& dndu, const exrVector3& dndv, const Shape* shape)
    : Interaction(point, (Cross(dpdu, dpdv)).Normalized(), wo)
    , m_Dpdu(dpdu)
    , m_Dpdv(dpdv)
    , m_Dndu(dndu)
    , m_Dndv(dndv)
    , m_Shape(shape)
    , m_BSDF(nullptr)
{
    m_ShadingInfo.m_Normal = m_Normal;
    m_ShadingInfo.m_Dpdu = dpdu;
    m_ShadingInfo.m_Dpdv = dpdv;
    m_ShadingInfo.m_Dndu = dndu;
    m_ShadingInfo.m_Dndv = dndv;
};

void SurfaceInteraction::SetShadingGeometry(const exrVector3& sDpdu, const exrVector3& sDpdv,
    const exrVector3& sDndu, const exrVector3& sDndv)
{
    m_ShadingInfo.m_Normal = (Cross(sDpdu, sDpdv)).Normalized();
    m_ShadingInfo.m_Dpdu = sDpdu;
    m_ShadingInfo.m_Dpdv = sDpdv;
    m_ShadingInfo.m_Dndu = sDndu;
    m_ShadingInfo.m_Dndv = sDndv;
}

void SurfaceInteraction::ComputeDifferentials(const RayDifferential& ray)
{
    if (ray.m_HasDifferentials)
    {
        throw "Ray differentials have yet to be implemented!";
    }
    else
    {
        m_Dudx = 0;
        m_Dvdx = 0;
        m_Dudy = 0;
        m_Dvdy = 0;
        m_Dpdx = exrVector3::Zero();
        m_Dpdy = exrVector3::Zero();
    }
}

void SurfaceInteraction::ComputeScatteringFunctions(const RayDifferential& ray)
{
    ComputeDifferentials(ray);
    m_Primitive->ComputeScatteringFunctions(this);
}

exrEND_NAMESPACE
