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
#include "math/math.h"

exrBEGIN_NAMESPACE

//! Return a random point in a unit disc with using the rejection sampling
//! technique. While it may be tempting to just generate two x and y values,
//! it would not have a uniform distribution across the disc.
//!
//! @return a random point on a unit disc
inline exrVector2 RejectionSampleDisc()
{



}

inline exrVector3 UniformSampleHemisphere(const exrPoint2& u) 
{
    exrFloat z = u.x;
    exrFloat r = sqrt(exrMax(0.0f, 1.0f - z * z));
    exrFloat phi = 2 * EXR_M_PI * u.y;
    return exrVector3(r * cos(phi), r * sin(phi), z);
}

inline exrFloat UniformHemispherePdf() {
    return EXR_M_INV2PI;
}

inline exrVector3 UniformSampleSphere(const exrPoint2& u)
{
    exrFloat z = 1 - 2 * u.x;
    exrFloat r = sqrt(exrMax(0.0f, 1.0f - z * z));
    exrFloat phi = 2 * EXR_M_PI * u.y;
    return exrVector3(r * cos(phi), r * sin(phi), z);
}

inline exrFloat UniformSpherePdf()
{
    return EXR_M_INV2PI;
}

inline exrPoint2 ConcentricSampleDisk()
{
    exrPoint2 uOffset = 2.0f * exrPoint2(Random::UniformFloat(), Random::UniformFloat()) - exrVector2(1, 1);

    if (uOffset.x == 0 && uOffset.y == 0)
        return exrPoint2(0, 0);

    exrFloat theta, r;

    if (abs(uOffset.x) > abs(uOffset.y)) {
        r = uOffset.x;
        theta = EXR_M_PIOVER4 * (uOffset.y / uOffset.x);
    }
    else {
        r = uOffset.y;
        theta = EXR_M_PIOVER2 - EXR_M_PIOVER4 * (uOffset.x / uOffset.y);
    }

    return r * exrPoint2(cos(theta), sin(theta));
}

inline exrVector3 CosineSampleHemisphere()
{
    exrPoint2 d = ConcentricSampleDisk();
    exrFloat z = sqrt(exrMax(0.0f, 1.0f - d.x * d.x - d.y * d.y));
    return exrVector3(d.x, d.y, z);
}

inline exrFloat CosineHemispherePdf(exrFloat cosTheta) 
{
    return cosTheta * EXR_M_INVPI;
}

exrEND_NAMESPACE