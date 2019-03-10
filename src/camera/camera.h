#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math\ray.h"

class Camera
{
public:
    Camera(Vector3f position, Vector3f lookat, Vector3f up, float vfov, float aspect, float aperture, float focusDist) {
        // virtual lens to simulate defocus blur
        lensRadius = aperture / 2.0f;
               
        float theta = vfov * float(M_PI) / 180.f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = aspect * halfHeight;

        m_Position = position;
        w = (lookat - position).Normalized(); // look in positive -z axis, RH coordinate system
        u = (Cross(w, up)).Normalized();
        v = Cross(u, w);

        m_Min = position - halfWidth * focusDist * u - halfHeight * focusDist * v + w * focusDist;
        m_HorizontalStep = 2.0f * halfWidth * focusDist* u;
        m_VerticalStep = 2.0f * halfHeight * focusDist * v;
    }

    Ray GetViewRay(float s, float t) 
    { 
        Vector3f rd = lensRadius * RandomInUnitDisc();
        Vector3f offset = u * rd.x + v * rd.y;
        return Ray(m_Position + offset, m_Min + s * m_HorizontalStep + t * m_VerticalStep - m_Position - offset); 
    }

    Vector3f m_Position;
    Vector3f m_Min;
    Vector3f m_HorizontalStep;
    Vector3f m_VerticalStep;

    Vector3f u, v, w;

    float lensRadius;
};
#endif // !__CAMERA_H__

