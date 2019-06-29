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

//! @brief A class that handles various pseudo random value generations
class Random
{
public:
    //! @brief Seeds the random class
    //! @param a                The value to seed the random class
    static inline void Seed(int a) { srand(a); }

    //! @brief Generates a random value between 0 and 1
    //! @return A random exrFloat between 0 and 1
    static inline exrFloat Uniform01() { return (exrFloat)rand() / RAND_MAX; }

    //! @brief Generates a vector between length 0 - 1 in a random direction
    //! @return A random vector inside a unit sphere
    static exrVector3 RandomInUnitSphere();

    //! @brief Generates a vector on the surface of a unit sphere
    //! @return A random vector on the surface of a unit sphere
    static exrVector3 RandomOnUnitSphere();

    static exrPoint2 Uniform01Point2() { return exrPoint2(Uniform01(), Uniform01()); }

    static exrVector3 UniformSampleHemisphere(const exrPoint2& u) {
        exrFloat z = u.x;
        exrFloat r = sqrt(exrMax(0.0f, 1.0f - z * z));
        exrFloat phi = 2 * EXR_M_PI * u.y;
        return exrVector3(r * cos(phi), r * sin(phi), z);
    }

    static exrFloat UniformHemispherePdf() {
        return EXR_M_INV2PI;
    }

    static exrVector3 UniformSampleSphere(const exrPoint2& u) {
        exrFloat z = 1 - 2 * u.x;
        exrFloat r = sqrt(exrMax(0.0f, 1.0f - z * z));
        exrFloat phi = 2 * EXR_M_PI * u.y;
        return exrVector3(r * cos(phi), r * sin(phi), z);
    }

    static exrFloat UniformSpherePdf() {
        return EXR_M_INV2PI;
    }

    static exrPoint2 ConcentricSampleDisk(const exrPoint2& u) {
        exrPoint2 uOffset = 2.0f * u - exrVector2(1, 1);

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

    static exrVector3 CosineSampleHemisphere(const exrPoint2& u) {
        exrPoint2 d = ConcentricSampleDisk(u);
        exrFloat z = sqrt(exrMax(0.0f, 1.0f - d.x * d.x - d.y * d.y));
        return exrVector3(d.x, d.y, z);
    }

    static exrFloat CosineHemispherePdf(exrFloat cosTheta) {
        return cosTheta * EXR_M_INVPI;
    }

};

exrEND_NAMESPACE