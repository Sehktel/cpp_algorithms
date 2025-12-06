#pragma once
#include "../../core/math/matrix4.h"
#include "../../core/math/vector3.h"

// Аффинные преобразования
class AffineTransform
{
public:
    // Применяет преобразование к точке
    static void transformPoint(const Matrix4& transform, const float* point, float* result);
    
    // Применяет преобразование к вектору (без переноса)
    static void transformVector(const Matrix4& transform, const float* vector, float* result);
    
    // Применяет преобразование к нормали (обратная транспонированная матрица)
    static void transformNormal(const Matrix4& transform, const float* normal, float* result);
    
    // Композиция преобразований: T * R * S
    static Matrix4 compose(const float* translation, const float* rotation, const float* scale);
    
    // Декомпозиция матрицы на компоненты
    static void decompose(const Matrix4& transform, float* translation, float* rotation, float* scale);
    
    // Поворот точки вокруг оси
    static void rotatePointAroundAxis(const float* point, const float* axis, float angle, float* result);
    
    // Поворот точки вокруг точки
    static void rotatePointAroundPoint(const float* point, const float* center, const float* axis, float angle, float* result);
};

