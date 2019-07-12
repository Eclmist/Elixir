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

#include "mirror.h"

exrBEGIN_NAMESPACE

exrSpectrum Mirror::Evaluate(const exrVector3& wo, const exrVector3& wi) const
{
    if ((wi - Reflect(wo, exrVector3::Forward())).Magnitude() <= EXR_EPSILON)
        return m_SpecularTint;

    return 0;
}

void Mirror::Sample(const exrVector3& wo, exrVector3* wi, exrFloat* pdf, BxDFType flags) const
{
    // local space normal is always z forward
    *wi = Reflect(wo, exrVector3::Forward());
    *pdf = 1;
}

exrEND_NAMESPACE

