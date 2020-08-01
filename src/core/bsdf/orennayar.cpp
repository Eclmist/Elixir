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

#include "orennayar.h"
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

IMPLEMENT_CLOSURE_TYPE_BEGIN(ClosureTypeOrenNayar)
IMPLEMENT_CLOSURE_TYPE_VAR(ClosureTypeOrenNayar, Tsl_float3, albedo)
IMPLEMENT_CLOSURE_TYPE_VAR(ClosureTypeOrenNayar, Tsl_float, roughness)
IMPLEMENT_CLOSURE_TYPE_END(ClosureTypeOrenNayar)

exrSpectrum OrenNayar::f(const exrVector3& wo, const exrVector3& wi) const
{
    exrFloat sinThetaI = SinTheta(wi);
    exrFloat sinThetaO = SinTheta(wo);

    // cos(a - b) = cos(a)*cos(b) + sin(a)*sin(b)
    exrFloat maxCos = 0;
    if (sinThetaI > EXR_EPSILON && sinThetaO > EXR_EPSILON)
    {
        exrFloat sinPhiI = SinPhi(wi);
        exrFloat cosPhiI = CosPhi(wi);
        exrFloat sinPhiO = SinPhi(wo);
        exrFloat cosPhiO = CosPhi(wo);
        exrFloat dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
        maxCos = exrMax(maxCos, dCos);
    }

    exrFloat sinAlpha, tanBeta;
    if (AbsCosTheta(wi) > AbsCosTheta(wo))
    {
        sinAlpha = sinThetaO;
        tanBeta = sinThetaI / AbsCosTheta(wi);
    }
    else
    {
        sinAlpha = sinThetaI;
        tanBeta = sinThetaO / AbsCosTheta(wo);
    }

    return m_Albedo * EXR_M_INVPI * (m_A + m_B * maxCos * sinAlpha * tanBeta);
}

exrEND_NAMESPACE