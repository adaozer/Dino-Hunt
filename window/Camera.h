#pragma once
#include "maths.h"

class Camera {
public:
    Vec3 pos = Vec3(0.f, 0.f, 5.f);

    float yaw = 0.f;
    float pitch = 0.f;

    float moveSpeed = 5.f;
    float sensitivity = 0.0025f;

    Vec3 up = Vec3(0, 1, 0);

    Vec3 front() const {
        return Vec3(std::cos(pitch) * std::cos(yaw), std::sin(pitch), std::cos(pitch) * std::sin(yaw)).normalize();
    }

    Vec3 right() const {
        return Vec3::Cross(up, front()).normalize();
    }

    Matrix viewMatrix() const {
        return Matrix::lookAt(pos, pos + front(), up);
    }
};
