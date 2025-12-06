#pragma once

// Заполнение полигонов
class PolygonFiller
{
public:
    // Заполняет полигон методом scanline
    // points - массив точек (x, y), numPoints - количество точек
    static void scanlineFill(int* points, int numPoints,
                             unsigned char* framebuffer, int width, int height,
                             unsigned char r, unsigned char g, unsigned char b);
    
    // Заполняет полигон методом flood fill
    static void floodFill(int startX, int startY,
                          unsigned char* framebuffer, int width, int height,
                          unsigned char r, unsigned char g, unsigned char b,
                          unsigned char targetR, unsigned char targetG, unsigned char targetB);
};

