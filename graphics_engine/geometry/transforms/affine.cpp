#include "affine.h"
#include <cmath>

void AffineTransform::transformPoint(const Matrix4& transform, const float* point, float* result)
{
    transform.multiplyPoint(point, result);
}

void AffineTransform::transformVector(const Matrix4& transform, const float* vector, float* result)
{
    transform.multiplyVector(vector, result);
}

void AffineTransform::transformNormal(const Matrix4& transform, const float* normal, float* result)
{
    // Для нормалей используем обратную транспонированную матрицу
    // Упрощенная версия: транспонированная обратная верхней левой 3x3 части
    Matrix4 invTrans = transform.inverse().transpose();
    invTrans.multiplyVector(normal, result);
    
    // Нормализуем результат
    float len = std::sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);
    if (len > 0.0001f)
    {
        result[0] /= len;
        result[1] /= len;
        result[2] /= len;
    }
}

Matrix4 AffineTransform::compose(const float* translation, const float* rotation, const float* scale)
{
    // Порядок: T * R * S
    Matrix4 T = Matrix4::translation(translation[0], translation[1], translation[2]);
    Matrix4 R = Matrix4::rotationEuler(rotation[0], rotation[1], rotation[2]);
    Matrix4 S = Matrix4::scale(scale[0], scale[1], scale[2]);
    
    return T * R * S;
}

void AffineTransform::decompose(const Matrix4& transform, float* translation, float* rotation, float* scale)
{
    // Извлекаем перенос
    translation[0] = transform.m[12];
    translation[1] = transform.m[13];
    translation[2] = transform.m[14];
    
    // Извлекаем масштаб из длины векторов
    float sx = std::sqrt(transform.m[0]*transform.m[0] + transform.m[1]*transform.m[1] + transform.m[2]*transform.m[2]);
    float sy = std::sqrt(transform.m[4]*transform.m[4] + transform.m[5]*transform.m[5] + transform.m[6]*transform.m[6]);
    float sz = std::sqrt(transform.m[8]*transform.m[8] + transform.m[9]*transform.m[9] + transform.m[10]*transform.m[10]);
    
    scale[0] = sx;
    scale[1] = sy;
    scale[2] = sz;
    
    // Извлекаем поворот (упрощенная версия)
    rotation[0] = std::atan2(transform.m[6], transform.m[10]);  // pitch
    rotation[1] = std::asin(-transform.m[2]);                  // yaw
    rotation[2] = std::atan2(transform.m[1], transform.m[0]);  // roll
}

void AffineTransform::rotatePointAroundAxis(const float* point, const float* axis, float angle, float* result)
{
    // Поворот точки вокруг оси через начало координат
    Matrix4 rot = Matrix4::rotationAxis(axis, angle);
    rot.multiplyPoint(point, result);
}

void AffineTransform::rotatePointAroundPoint(const float* point, const float* center, const float* axis, float angle, float* result)
{
    // Переносим точку в начало координат
    float translated[3] = {
        point[0] - center[0],
        point[1] - center[1],
        point[2] - center[2]
    };
    
    // Поворачиваем
    float rotated[3];
    rotatePointAroundAxis(translated, axis, angle, rotated);
    
    // Возвращаем обратно
    result[0] = rotated[0] + center[0];
    result[1] = rotated[1] + center[1];
    result[2] = rotated[2] + center[2];
}

