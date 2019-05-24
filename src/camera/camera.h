#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/elixir.h"

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
    Camera(exrPoint position, exrPoint lookat, exrVector3 up, exrFloat vfov, exrFloat aspect, exrFloat aperture, exrFloat focusDist) {
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
    }

    //! @brief Creates a view ray based from a uv coordinate
    //!
    //! @param s                The u coordinate of the ray in screen space
    //! @param t                The v coordinate of the ray in screen space
    Ray GetViewRay(exrFloat s, exrFloat t) 
    { 
        exrVector3 rd = lensRadius * Random::RandomInUnitDisc();
        exrVector3 offset = u * rd.x + v * rd.y;
        return Ray(m_Position + offset, m_Min + s * m_HorizontalStep + t * m_VerticalStep - m_Position - offset); 
    }

    exrPoint m_Position;
    exrPoint m_Min;
    exrVector3 m_HorizontalStep;
    exrVector3 m_VerticalStep;

    exrVector3 u, v, w;
    exrFloat lensRadius;
};

exrEND_NAMESPACE

#endif // !__CAMERA_H__