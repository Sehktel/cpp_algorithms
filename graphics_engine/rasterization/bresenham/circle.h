#pragma once

// Алгоритм Брезенхема для рисования окружностей
class BresenhamCircle
{
public:
    // Рисует окружность с центром (xc, yc) и радиусом r
    static void draw(int xc, int yc, int r,
                     void (*callback)(int x, int y));
    
    // Рисует окружность с цветом
    static void draw(int xc, int yc, int r,
                     unsigned char* framebuffer, int width, int height,
                     unsigned char r_color, unsigned char g, unsigned char b);
    
    // Рисует заполненную окружность
    static void drawFilled(int xc, int yc, int r,
                           unsigned char* framebuffer, int width, int height,
                           unsigned char r_color, unsigned char g, unsigned char b);
};

