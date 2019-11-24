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

#include "pointlight.h"

exrBEGIN_NAMESPACE

exrSpectrum PointLight::Sample_Li(const Interaction& ref, exrVector3& wi, exrFloat& pdf,
    VisibilityTester* vis) const
{
    *vis = VisibilityTester(ref, Interaction(m_Transform.GetPosition()));
    wi = (m_Transform.GetPosition() - ref.m_Point).Normalized();
    pdf = 1.0f;
    return m_Intensity / DistanceSquared(m_Transform.GetPosition(), ref.m_Point);
}

exrSpectrum PointLight::Power() const
{
    return m_Intensity * 4 * EXR_M_PI;
}
exrEND_NAMESPACE