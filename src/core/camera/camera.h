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

/* ========================================================================================== /
         ######\  ##\      ########\  ######\  ##\   ##\       ##\      ##\ ########\
        ##  __##\ ## |     ##  _____|##  __##\ ###\  ## |      ###\    ### |##  _____|
        ## /  \__|## |     ## |      ## /  ## |####\ ## |      ####\  #### |## |
        ## |      ## |     #####\    ######## |## ##\## |      ##\##\## ## |#####\
        ## |      ## |     ##  __|   ##  __## |## \#### |      ## \###  ## |##  __|
        ## |  ##\ ## |     ## |      ## |  ## |## |\### |      ## |\#  /## |## |
        \######  |########\########\ ## |  ## |## | \## |      ## | \_/ ## |########\
         \______/ \________\________|\__|  \__|\__|  \__|      \__|     \__|\________|
                 Banner of shame. Remove when this file is no longer ghetto.
=========================================================================================== */

#pragma once

#include "core/elixir.h"
#include "core/film/film.h"

exrBEGIN_NAMESPACE

//! @brief A simple look-at camera that handles view rays generatino
//!
//! The camera class can be positioned using an origin and a look at point. FOV, aperture, focus distance
//! are some of the settings that can be adjusted such that the generated ray can be used in effects
//! such as motion blur or defocus blur.
class Camera
{
public:
    //! @brief Creates a camera object
    //!
    //! @param position          The position of the camera
    //! @param lookat            The look at point of the camera
    //! @param up                The camera's up vector, used to adjust roll
    //! @param vfov              The field of view in degrees
    //! @param aspect            The aspect ratio of the expected output
    //! @param aperture          The aperture of the camera
    //! @param focusDist         The distance away from the camera's focus plane
    Camera(exrPoint3 position, exrPoint3 lookat, exrVector3 up, exrFloat vfov, exrFloat aspect, exrFloat aperture, exrFloat focusDist) {
        // virtual lens to simulate defocus blur
        lensRadius = aperture / 2.0f;
               
        exrFloat theta = vfov * exrFloat(EXR_M_PI) / 180.f;
        exrFloat halfHeight = tan(theta / 2.0f);
        exrFloat halfWidth = aspect * halfHeight;

        m_Position = position;
        w = (lookat - position).Normalized(); // look in positive -z axis, RH coordinate system
        u = (Cross(w, up)).Normalized();
        v = Cross(u, w);

        m_Min = position - halfWidth * focusDist * u - halfHeight * focusDist * v + w * focusDist;
        m_HorizontalStep = 2.0f * halfWidth * focusDist* u;
        m_VerticalStep = 2.0f * halfHeight * focusDist * v;

        // TODO: replace these with values from renderjob
        m_Film = std::make_unique<Film>(Point2<exrU32>(512, 512), g_RuntimeOptions.outputFile, g_RuntimeOptions.stampFile);
    }

    //! @brief Creates a view ray based from a uv coordinate
    //!
    //! @param s                The u coordinate of the ray in screen space
    //! @param t                The v coordinate of the ray in screen space
    RayDifferential GetViewRay(exrFloat s, exrFloat t) 
    { 
        exrPoint2 randomDiscOffset = ConcentricSampleDisk(Uniform01Point2());
        exrVector3 rd = lensRadius * exrVector3(randomDiscOffset.x, randomDiscOffset.y, 0);
        exrVector3 offset = u * rd.x + v * rd.y;
        return RayDifferential(m_Position + offset, m_Min + s * m_HorizontalStep + t * m_VerticalStep - m_Position - offset); 
    }

    exrPoint3 m_Position;
    exrPoint3 m_Min;
    exrVector3 m_HorizontalStep;
    exrVector3 m_VerticalStep;

    exrVector3 u, v, w;
    exrFloat lensRadius;

    std::unique_ptr<Film> m_Film;
};

exrEND_NAMESPACE