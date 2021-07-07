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

#include "camera.h"

exrBEGIN_NAMESPACE

Camera::Camera(exrPoint3 position, exrPoint3 lookat, exrVector3 up, exrFloat vfov, exrFloat aperture, exrFloat focusDist) {
    // virtual lens to simulate defocus blur
    lensRadius = aperture / 2.0f;
            
    exrFloat theta = vfov * exrFloat(EXR_M_PI) / 180.f;
    exrFloat halfHeight = tan(theta / 2.0f);
    exrFloat aspect = WIDTH / (exrFloat)HEIGHT;
    exrFloat halfWidth = aspect * halfHeight;

    m_Position = position;
    w = (lookat - position).Normalized(); // look in positive -z axis, RH coordinate system
    u = (Cross(w, up)).Normalized();
    v = Cross(u, w);

    m_Min = position - halfWidth * focusDist * u - halfHeight * focusDist * v + w * focusDist;
    m_HorizontalStep = 2.0f * halfWidth * focusDist * u;
    m_VerticalStep = 2.0f * halfHeight * focusDist * v;

    m_Exporter = std::make_unique<Exporter>(Point2<exrU32>(WIDTH, HEIGHT), g_RuntimeOptions.outputFile, g_RuntimeOptions.stampFile);
}

Ray Camera::GetViewRay(exrFloat s, exrFloat t) 
{ 
    exrPoint2 randomDiscOffset = RejectionSampleDisk();
    exrVector3 rd = lensRadius * exrVector3(randomDiscOffset.x, randomDiscOffset.y, 0);
    exrVector3 offset = u * rd.x + v * rd.y;
    return Ray(m_Position + offset, m_Min + s * m_HorizontalStep + t * m_VerticalStep - m_Position - offset); 
}

exrEND_NAMESPACE