#pragma once
#include "maths.h"
#include <cmath>

class Camera {
public:
    Vec3 position;

    float yaw;
    float pitch;

    float moveSpeed;
    float sensitivity;

    Camera(Vec3 _pos) : position(_pos), yaw(0.f), pitch(0.f), moveSpeed(5.f), sensitivity(0.0025f) {}

    Vec3 forward() const {
        Vec3 f(std::cos(pitch) * std::cos(yaw), std::sin(pitch), std::cos(pitch) * std::sin(yaw));
        return f.normalize();     
    }

    Vec3 right() const {
        Vec3 fwd = forward();
        Vec3 up(0.0f, 1.0f, 0.0f);
        Vec3 r = Vec3::Cross(up, fwd).normalize();
        return r;
    }

    Matrix viewMatrix() const {
        Vec3 fwd = forward();
        Vec3 target = position + fwd;

        return Matrix::lookAt(target, position, Vec3(0,1,0));
    }
};
