#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "math\ray.h"

class Camera
{
public:
    Camera() {
        m_Position = Vector3f::Zero();
        m_Min = Vector3f(-2.0f, -1.0f, -1.0f);
        m_VerticalStep = Vector3f(0.0f, 2.0f, 0.0f);
        m_HorizontalStep = Vector3f(4.0f, 0.0f, 0.0f);
    }

    Ray GetViewRay(float u, float v) { return Ray(m_Position, m_Min + u * m_HorizontalStep + v * m_VerticalStep - m_Position); }

    Vector3f m_Position;
    Vector3f m_Min;
    Vector3f m_HorizontalStep;
    Vector3f m_VerticalStep;
};
#endif // !__CAMERA_H__

