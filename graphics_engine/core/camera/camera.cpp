#include "camera.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera()
{
    position[0] = 0.0f; position[1] = 0.0f; position[2] = 5.0f;
    target[0] = 0.0f; target[1] = 0.0f; target[2] = 0.0f;
    up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
    fov = 45.0f * M_PI / 180.0f;
    aspect = 16.0f / 9.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;
}

void Camera::setPosition(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Camera::setTarget(float x, float y, float z)
{
    target[0] = x;
    target[1] = y;
    target[2] = z;
}

void Camera::setUp(float x, float y, float z)
{
    up[0] = x;
    up[1] = y;
    up[2] = z;
}

void Camera::setProjection(float fovDegrees, float aspectRatio, float near, float far)
{
    fov = fovDegrees * M_PI / 180.0f;
    aspect = aspectRatio;
    nearPlane = near;
    farPlane = far;
}

Matrix4 Camera::getViewMatrix() const
{
    return Matrix4::lookAt(position, target, up);
}

Matrix4 Camera::getProjectionMatrix() const
{
    return Matrix4::perspective(fov, aspect, nearPlane, farPlane);
}

