#pragma once

// Алгоритм Брезенхема для рисования линий
class BresenhamLine
{
public:
    // Рисует линию от (x0, y0) до (x1, y1)
    // callback вызывается для каждой точки (x, y)
    static void draw(int x0, int y0, int x1, int y1, 
                     void (*callback)(int x, int y));
    
    // Рисует линию с цветом
    static void draw(int x0, int y0, int x1, int y1,
                     unsigned char* framebuffer, int width, int height,
                     unsigned char r, unsigned char g, unsigned char b);
};

