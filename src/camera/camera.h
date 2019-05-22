#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/system/system.h"
#include "math/ray.h"
#include "math/random.h"

exrBEGIN_NAMESPACE

class Camera
{
public:
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