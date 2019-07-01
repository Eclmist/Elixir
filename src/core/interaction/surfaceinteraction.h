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

#include "interaction.h"

exrBEGIN_NAMESPACE

class Shape;
class BSDF;
class RayDifferential;

class SurfaceInteraction : public Interaction
{
public:

    struct ShadingInfo
    {
        exrVector3 m_Normal;
        exrVector3 m_Dpdu, m_Dpdv;
        exrVector3 m_Dndu, m_Dndv;
    };

    SurfaceInteraction() {};
    SurfaceInteraction(const exrPoint3& point, const exrVector3& wo, const exrVector3& dpdu, const exrVector3& dpdv,
        const exrVector3& dndu, const exrVector3& dndv, const Shape* shape);

    void SetShadingGeometry(const exrVector3& sDpdu, const exrVector3& sDpdv, const exrVector3& sDndu,
        const exrVector3& sDndv);

    void ComputeDifferentials(const RayDifferential& ray);
    void ComputeScatteringFunctions(const RayDifferential& ray);

public:
    //! The BRDF of the surface
    BSDF* m_BSDF = nullptr;

    ShadingInfo m_ShadingInfo;

    // Partial derivatives that describe the surface
    exrVector3 m_Dpdu, m_Dpdv;
    exrVector3 m_Dndu, m_Dndv;

    //! A reference to the shape that the interaction lies on
    const Primitive* m_Primitive = nullptr;
    const Shape* m_Shape = nullptr;

    mutable exrVector3 m_Dpdx, m_Dpdy;
    mutable exrFloat m_Dudx = 0, m_Dvdx = 0, m_Dudy = 0, m_Dvdy = 0;
};

exrEND_NAMESPACE
