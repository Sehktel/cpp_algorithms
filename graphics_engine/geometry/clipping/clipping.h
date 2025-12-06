#pragma once

// Отсечение (Clipping)
class Clipping
{
public:
    // Отсечение линии по границам экрана (алгоритм Коэна-Сазерленда)
    static bool clipLine(int& x0, int& y0, int& x1, int& y1,
                        int xMin, int yMin, int xMax, int yMax);
    
    // Отсечение полигона (алгоритм Сазерленда-Ходжмана)
    static void clipPolygon(const float* vertices, int numVertices,
                           float xMin, float yMin, float xMax, float yMax,
                           float* outVertices, int& outNumVertices);
    
    // Back-face culling (отсечение невидимых граней)
    static bool isBackFace(const float* v0, const float* v1, const float* v2,
                          const float* cameraPos);
};

