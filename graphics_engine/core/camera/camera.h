#pragma once
#include "../math/matrix4.h"

// Камера
class Camera
{
private:
    float position[3];
    float target[3];
    float up[3];
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;
    
public:
    Camera();
    
    void setPosition(float x, float y, float z);
    void setTarget(float x, float y, float z);
    void setUp(float x, float y, float z);
    void setProjection(float fov, float aspect, float nearPlane, float farPlane);
    
    Matrix4 getViewMatrix() const;
    Matrix4 getProjectionMatrix() const;
    
    const float* getPosition() const { return position; }
    const float* getTarget() const { return target; }
};

