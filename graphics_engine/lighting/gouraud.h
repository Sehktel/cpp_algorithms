#pragma once

// Модель освещения Гуро (интерполяция цветов по вершинам)
class GouraudLighting
{
public:
    // Вычисляет цвет вершины
    static void calculateVertexColor(const float* position,
                                     const float* normal,
                                     const float* lightPos,
                                     const float* lightColor,
                                     float* outColor);
    
    // Интерполирует цвет по треугольнику
    static void interpolateColor(float x, float y,
                                const float* v0, const float* v1, const float* v2,  // Позиции вершин
                                const float* c0, const float* c1, const float* c2,  // Цвета вершин
                                float* outColor);
};

