#include "clipping.h"
#include "../../core/math/vector3.h"
#include <algorithm>
#include <cmath>

// Коды областей для алгоритма Коэна-Сазерленда
enum OutCode
{
    INSIDE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTTOM = 4,
    TOP = 8
};

static OutCode computeOutCode(int x, int y, int xMin, int yMin, int xMax, int yMax)
{
    OutCode code = INSIDE;
    
    if (x < xMin)
        code |= LEFT;
    else if (x > xMax)
        code |= RIGHT;
    
    if (y < yMin)
        code |= BOTTOM;
    else if (y > yMax)
        code |= TOP;
    
    return code;
}

bool Clipping::clipLine(int& x0, int& y0, int& x1, int& y1,
                       int xMin, int yMin, int xMax, int yMax)
{
    // Алгоритм Коэна-Сазерленда
    OutCode outCode0 = computeOutCode(x0, y0, xMin, yMin, xMax, yMax);
    OutCode outCode1 = computeOutCode(x1, y1, xMin, yMin, xMax, yMax);
    
    while (true)
    {
        if (!(outCode0 | outCode1))
        {
            // Оба конца внутри
            return true;
        }
        
        if (outCode0 & outCode1)
        {
            // Оба конца с одной стороны от границы
            return false;
        }
        
        // Выбираем точку вне границы
        OutCode outCodeOut = outCode0 ? outCode0 : outCode1;
        int x, y;
        
        // Находим точку пересечения
        if (outCodeOut & TOP)
        {
            x = x0 + (x1 - x0) * (yMax - y0) / (y1 - y0);
            y = yMax;
        }
        else if (outCodeOut & BOTTOM)
        {
            x = x0 + (x1 - x0) * (yMin - y0) / (y1 - y0);
            y = yMin;
        }
        else if (outCodeOut & RIGHT)
        {
            y = y0 + (y1 - y0) * (xMax - x0) / (x1 - x0);
            x = xMax;
        }
        else if (outCodeOut & LEFT)
        {
            y = y0 + (y1 - y0) * (xMin - x0) / (x1 - x0);
            x = xMin;
        }
        
        // Обновляем точку
        if (outCodeOut == outCode0)
        {
            x0 = x;
            y0 = y;
            outCode0 = computeOutCode(x0, y0, xMin, yMin, xMax, yMax);
        }
        else
        {
            x1 = x;
            y1 = y;
            outCode1 = computeOutCode(x1, y1, xMin, yMin, xMax, yMax);
        }
    }
}

bool Clipping::isBackFace(const float* v0, const float* v1, const float* v2,
                          const float* cameraPos)
{
    // Вычисляем нормаль треугольника
    Vector3 edge1(v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]);
    Vector3 edge2(v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]);
    Vector3 normal = edge1.cross(edge2);
    
    // Вектор от треугольника к камере
    Vector3 toCamera(cameraPos[0] - v0[0], cameraPos[1] - v0[1], cameraPos[2] - v0[2]);
    
    // Если скалярное произведение отрицательное, грань невидима
    return normal.dot(toCamera) < 0;
}

void Clipping::clipPolygon(const float* vertices, int numVertices,
                          float xMin, float yMin, float xMax, float yMax,
                          float* outVertices, int& outNumVertices)
{
    // TODO: Реализовать алгоритм Сазерленда-Ходжмана
    // Упрощенная версия - просто копируем вершины
    outNumVertices = numVertices;
    for (int i = 0; i < numVertices * 3; i++)
    {
        outVertices[i] = vertices[i];
    }
}

