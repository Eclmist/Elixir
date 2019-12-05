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

#include "directionallight.h"

exrBEGIN_NAMESPACE

exrSpectrum DirectionalLight::Sample_Li(const Interaction& ref, exrVector3& wi, exrFloat& pdf,
    VisibilityTester* vis) const
{
    exrVector3 direction = m_Transform.GetMatrix() * -exrVector3::Up();
    *vis = VisibilityTester(ref, Interaction(ref.m_Point - direction * 999999999.9f));
    wi = -direction.Normalized();
    pdf = 1.0f;
    return m_Intensity;
}

exrSpectrum DirectionalLight::Power() const
{
    return m_Intensity;
}

exrEND_NAMESPACE