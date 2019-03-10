#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math\ray.h"

class Camera
{
public:
    Camera(Vector3f position, Vector3f lookat, Vector3f up, float vfov, float aspect) {
        Vector3f u, v, w;

        float theta = vfov * M_PI / 180.f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = aspect * halfHeight;

        m_Position = position;
        w = (position - lookat).Normalized();
        u = (Cross(w, up)).Normalized();
        v = Cross(u, w);

        m_Min = position - halfWidth * u - halfHeight * v - w;
        m_HorizontalStep = 2.0f * halfWidth * u;
        m_VerticalStep = 2.0f * halfHeight * v;
    }

    Ray GetViewRay(float u, float v) { return Ray(m_Position, m_Min + u * m_HorizontalStep + v * m_VerticalStep - m_Position); }

    Vector3f m_Position;
    Vector3f m_Min;
    Vector3f m_HorizontalStep;
    Vector3f m_VerticalStep;
};
#endif // !__CAMERA_H__

