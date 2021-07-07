/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019-2020 Samuel Van Allen - All rights reserved.

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

#include "fresnel.h"

exrBEGIN_NAMESPACE

exrFloat Fresnel::FrDielectric(exrFloat cosThetaI, exrFloat etaI, exrFloat etaT)
{
    cosThetaI = exrClamp(cosThetaI, -1, 1);

    if (cosThetaI < 0)
    {
        std::swap(etaI, etaT);
        cosThetaI = abs(cosThetaI);
    }

    // Compute cosThetaT using Snell's law
    exrFloat sinThetaI = sqrt(exrMax(0.0f, 1 - cosThetaI * cosThetaI));
    exrFloat sinThetaT = etaI / etaT * sinThetaI;

    // Total internal reflection
    if (sinThetaT >= 1)
        return 1;

    exrFloat cosThetaT = sqrt(exrMax(0.0f, 1 - sinThetaT * sinThetaT));

    exrFloat ntCosThetaI = etaT * cosThetaI;
    exrFloat ntCosThetaT = etaT * cosThetaT;
    exrFloat niCosThetaI = etaI * cosThetaI;
    exrFloat niCosThetaT = etaI * cosThetaT;


    exrFloat rParl = (ntCosThetaI - niCosThetaT) / (ntCosThetaI + niCosThetaT);
    exrFloat rPerp = (niCosThetaI - ntCosThetaT) / (niCosThetaI + ntCosThetaT);

    return 0.5f * (rParl * rParl + rPerp * rPerp);
}

exrFloat Fresnel::FrConductor(exrFloat cosThetaI, const exrSpectrum& etaI, const exrSpectrum& etaT, const exrSpectrum& k)
{
    exrError("Conductor Fresnel has yet to be implemented!");
    return 1;
}

// Introduction to "Physically Based Shading in Theory and Practice" by Naty Hoffman,
// Siggraph 2015 - https://www.youtube.com/watch?v=j-A0mwsJRmk (22:40)
// This is pretty much identical to schlick's approximation, except that f0
// is precomputed and stored as the material's specular color
exrSpectrum Fresnel::FrSchlick(exrSpectrum f0, exrFloat vDotH)
{
    return f0 + (exrSpectrum(1) - f0) * exrPow(1 - (vDotH), 5);
}

exrSpectrum FresnelDielectric::Evaluate(exrFloat cosThetaI) const
{
    return FrDielectric(cosThetaI, m_EtaI, m_EtaT);
}

exrEND_NAMESPACE

