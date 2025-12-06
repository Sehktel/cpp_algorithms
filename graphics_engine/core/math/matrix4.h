#pragma once

// Матрица 4x4 для преобразований
class Matrix4
{
public:
    float m[16];  // Хранится в row-major порядке
    
    Matrix4();
    Matrix4(const float* data);
    
    static Matrix4 identity();
    static Matrix4 translation(float x, float y, float z);
    static Matrix4 rotationX(float angle);
    static Matrix4 rotationY(float angle);
    static Matrix4 rotationZ(float angle);
    static Matrix4 rotationAxis(const float* axis, float angle);  // Поворот вокруг произвольной оси
    static Matrix4 rotationEuler(float yaw, float pitch, float roll);  // Эйлеровы углы
    static Matrix4 scale(float x, float y, float z);
    static Matrix4 perspective(float fov, float aspect, float near, float far);
    static Matrix4 lookAt(const float* eye, const float* target, const float* up);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);
    
    Matrix4 operator*(const Matrix4& other) const;
    void multiply(const float* in, float* out) const;  // Умножение на вектор (4D)
    void multiplyPoint(const float* in, float* out) const;  // Умножение точки (x,y,z,1)
    void multiplyVector(const float* in, float* out) const;  // Умножение вектора (x,y,z,0)
    
    Matrix4 transpose() const;
    Matrix4 inverse() const;
};

