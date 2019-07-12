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

#include "core/elixir.h"

exrBEGIN_NAMESPACE

class BxDF
{
public:

    enum BxDFType
    {
        BSDF_REFLECTION   = 1 << 0,
        BSDF_TRANSMISSION = 1 << 1,
        BSDF_DIFFUSE      = 1 << 2,
        BSDF_GLOSSY       = 1 << 3,
        BSDF_SPECULAR     = 1 << 4,
        BSDF_ALL          = BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR,
    };

    BxDF(BxDFType type)
        : m_BxDFType(type) {};

    exrBool MatchesFlags(BxDFType t) const { return (m_BxDFType & t) == m_BxDFType; };

    virtual exrSpectrum Evaluate(const exrVector3& wo, const exrVector3& wi) const = 0;
    virtual exrSpectrum Sample(const exrVector3& wo, exrVector3* wi, exrFloat* pdf, BxDFType flags) const;
    virtual exrFloat Pdf(const exrVector3& wo, const exrVector3& wi) const;

    inline exrBool IsSameHemisphere(const exrVector3& wo, const exrVector3& wi) const { return wo.z * wi.z > 0; };
private:
    BxDFType m_BxDFType;
};

exrEND_NAMESPACE