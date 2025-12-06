#pragma once

// Трехмерный вектор
struct Vector3
{
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;
    float dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    float length() const;
    Vector3 normalize() const;
};

