#include "matrix4.h"
#include <cmath>
#include <cstring>

Matrix4::Matrix4()
{
    std::memset(m, 0, 16 * sizeof(float));
}

Matrix4::Matrix4(const float* data)
{
    std::memcpy(m, data, 16 * sizeof(float));
}

Matrix4 Matrix4::identity()
{
    Matrix4 result;
    result.m[0] = result.m[5] = result.m[10] = result.m[15] = 1.0f;
    return result;
}

Matrix4 Matrix4::translation(float x, float y, float z)
{
    Matrix4 result = identity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

Matrix4 Matrix4::rotationX(float angle)
{
    Matrix4 result = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;
    return result;
}

Matrix4 Matrix4::rotationY(float angle)
{
    Matrix4 result = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;
    return result;
}

Matrix4 Matrix4::rotationZ(float angle)
{
    Matrix4 result = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
}

Matrix4 Matrix4::scale(float x, float y, float z)
{
    Matrix4 result = identity();
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
}

Matrix4 Matrix4::perspective(float fov, float aspect, float near, float far)
{
    Matrix4 result;
    float f = 1.0f / std::tan(fov / 2.0f);
    
    result.m[0] = f / aspect;
    result.m[5] = f;
    result.m[10] = (far + near) / (near - far);
    result.m[11] = -1.0f;
    result.m[14] = (2.0f * far * near) / (near - far);
    
    return result;
}

Matrix4 Matrix4::rotationAxis(const float* axis, float angle)
{
    // Поворот вокруг произвольной оси (формула Родрига)
    Matrix4 result = identity();
    
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    
    // Нормализуем ось
    float len = std::sqrt(x*x + y*y + z*z);
    if (len > 0.0001f)
    {
        x /= len;
        y /= len;
        z /= len;
    }
    
    float c = std::cos(angle);
    float s = std::sin(angle);
    float t = 1.0f - c;
    
    result.m[0] = t*x*x + c;
    result.m[1] = t*x*y + s*z;
    result.m[2] = t*x*z - s*y;
    
    result.m[4] = t*x*y - s*z;
    result.m[5] = t*y*y + c;
    result.m[6] = t*y*z + s*x;
    
    result.m[8] = t*x*z + s*y;
    result.m[9] = t*y*z - s*x;
    result.m[10] = t*z*z + c;
    
    return result;
}

Matrix4 Matrix4::rotationEuler(float yaw, float pitch, float roll)
{
    // Эйлеровы углы: yaw (Y), pitch (X), roll (Z)
    return rotationZ(roll) * rotationX(pitch) * rotationY(yaw);
}

Matrix4 Matrix4::lookAt(const float* eye, const float* target, const float* up)
{
    // Вычисляем базис камеры
    float forward[3] = {
        target[0] - eye[0],
        target[1] - eye[1],
        target[2] - eye[2]
    };
    
    float len = std::sqrt(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    if (len > 0.0001f)
    {
        forward[0] /= len;
        forward[1] /= len;
        forward[2] /= len;
    }
    
    // Правый вектор (right = forward × up)
    float right[3] = {
        forward[1] * up[2] - forward[2] * up[1],
        forward[2] * up[0] - forward[0] * up[2],
        forward[0] * up[1] - forward[1] * up[0]
    };
    
    len = std::sqrt(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
    if (len > 0.0001f)
    {
        right[0] /= len;
        right[1] /= len;
        right[2] /= len;
    }
    
    // Верхний вектор (up = right × forward)
    float upVec[3] = {
        right[1] * forward[2] - right[2] * forward[1],
        right[2] * forward[0] - right[0] * forward[2],
        right[0] * forward[1] - right[1] * forward[0]
    };
    
    // Строим матрицу вида
    Matrix4 result;
    result.m[0] = right[0];
    result.m[1] = upVec[0];
    result.m[2] = -forward[0];
    result.m[3] = 0;
    
    result.m[4] = right[1];
    result.m[5] = upVec[1];
    result.m[6] = -forward[1];
    result.m[7] = 0;
    
    result.m[8] = right[2];
    result.m[9] = upVec[2];
    result.m[10] = -forward[2];
    result.m[11] = 0;
    
    result.m[12] = -(right[0]*eye[0] + right[1]*eye[1] + right[2]*eye[2]);
    result.m[13] = -(upVec[0]*eye[0] + upVec[1]*eye[1] + upVec[2]*eye[2]);
    result.m[14] = -(-forward[0]*eye[0] - forward[1]*eye[1] - forward[2]*eye[2]);
    result.m[15] = 1;
    
    return result;
}

Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4 result = identity();
    
    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);
    
    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);
    
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++)
            {
                result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
            }
        }
    }
    return result;
}

void Matrix4::multiply(const float* in, float* out) const
{
    for (int i = 0; i < 4; i++)
    {
        out[i] = 0;
        for (int j = 0; j < 4; j++)
        {
            out[i] += m[i * 4 + j] * in[j];
        }
    }
}

void Matrix4::multiplyPoint(const float* in, float* out) const
{
    float vec[4] = {in[0], in[1], in[2], 1.0f};
    float result[4];
    multiply(vec, result);
    
    // Перспективное деление (если нужно)
    if (std::abs(result[3]) > 0.0001f)
    {
        out[0] = result[0] / result[3];
        out[1] = result[1] / result[3];
        out[2] = result[2] / result[3];
    }
    else
    {
        out[0] = result[0];
        out[1] = result[1];
        out[2] = result[2];
    }
}

void Matrix4::multiplyVector(const float* in, float* out) const
{
    float vec[4] = {in[0], in[1], in[2], 0.0f};
    multiply(vec, out);
    out[0] = out[0];
    out[1] = out[1];
    out[2] = out[2];
}

Matrix4 Matrix4::transpose() const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i * 4 + j] = m[j * 4 + i];
        }
    }
    return result;
}

Matrix4 Matrix4::inverse() const
{
    // TODO: Реализовать обратную матрицу (можно использовать метод Гаусса)
    // Пока возвращаем единичную матрицу
    return identity();
}

