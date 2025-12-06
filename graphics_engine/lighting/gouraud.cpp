#include "gouraud.h"
#include "../core/math/vector3.h"
#include <cmath>

void GouraudLighting::calculateVertexColor(const float* position,
                                           const float* normal,
                                           const float* lightPos,
                                           const float* lightColor,
                                           float* outColor)
{
    // Вектор от вершины к свету
    Vector3 lightDir(
        lightPos[0] - position[0],
        lightPos[1] - position[1],
        lightPos[2] - position[2]
    );
    lightDir = lightDir.normalize();
    
    // Нормаль
    Vector3 n(normal[0], normal[1], normal[2]);
    n = n.normalize();
    
    // Диффузное освещение
    float diff = std::max(0.0f, n.dot(lightDir));
    
    outColor[0] = lightColor[0] * diff;
    outColor[1] = lightColor[1] * diff;
    outColor[2] = lightColor[2] * diff;
    
    // Ограничиваем значения
    outColor[0] = std::min(1.0f, outColor[0]);
    outColor[1] = std::min(1.0f, outColor[1]);
    outColor[2] = std::min(1.0f, outColor[2]);
}

void GouraudLighting::interpolateColor(float x, float y,
                                       const float* v0, const float* v1, const float* v2,
                                       const float* c0, const float* c1, const float* c2,
                                       float* outColor)
{
    // Баррицентрические координаты
    float denom = (v1[1] - v2[1]) * (v0[0] - v2[0]) + (v2[0] - v1[0]) * (v0[1] - v2[1]);
    
    if (std::abs(denom) < 0.0001f)
    {
        outColor[0] = c0[0];
        outColor[1] = c0[1];
        outColor[2] = c0[2];
        return;
    }
    
    float w0 = ((v1[1] - v2[1]) * (x - v2[0]) + (v2[0] - v1[0]) * (y - v2[1])) / denom;
    float w1 = ((v2[1] - v0[1]) * (x - v2[0]) + (v0[0] - v2[0]) * (y - v2[1])) / denom;
    float w2 = 1.0f - w0 - w1;
    
    // Интерполяция цвета
    outColor[0] = c0[0] * w0 + c1[0] * w1 + c2[0] * w2;
    outColor[1] = c0[1] * w0 + c1[1] * w1 + c2[1] * w2;
    outColor[2] = c0[2] * w0 + c1[2] * w1 + c2[2] * w2;
}

