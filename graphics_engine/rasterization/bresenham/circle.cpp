#include "circle.h"
#include <cmath>

void BresenhamCircle::draw(int xc, int yc, int r,
                           void (*callback)(int x, int y))
{
    // Алгоритм Брезенхема для окружностей
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    // Рисуем 8 симметричных точек
    auto draw8Points = [&](int x, int y) {
        callback(xc + x, yc + y);
        callback(xc - x, yc + y);
        callback(xc + x, yc - y);
        callback(xc - x, yc - y);
        callback(xc + y, yc + x);
        callback(xc - y, yc + x);
        callback(xc + y, yc - x);
        callback(xc - y, yc - x);
    };
    
    draw8Points(x, y);
    
    while (y >= x)
    {
        x++;
        
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        
        draw8Points(x, y);
    }
}

void BresenhamCircle::draw(int xc, int yc, int r,
                           unsigned char* framebuffer, int width, int height,
                           unsigned char r_color, unsigned char g, unsigned char b)
{
    draw(xc, yc, r, [&](int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            int index = (y * width + x) * 3;
            framebuffer[index] = r_color;
            framebuffer[index + 1] = g;
            framebuffer[index + 2] = b;
        }
    });
}

void BresenhamCircle::drawFilled(int xc, int yc, int r,
                                  unsigned char* framebuffer, int width, int height,
                                  unsigned char r_color, unsigned char g, unsigned char b)
{
    // Рисуем заполненную окружность, рисуя горизонтальные линии
    for (int y = yc - r; y <= yc + r; y++)
    {
        if (y < 0 || y >= height)
            continue;
        
        int dx = std::sqrt(r * r - (y - yc) * (y - yc));
        int x1 = xc - dx;
        int x2 = xc + dx;
        
        for (int x = x1; x <= x2; x++)
        {
            if (x >= 0 && x < width)
            {
                int index = (y * width + x) * 3;
                framebuffer[index] = r_color;
                framebuffer[index + 1] = g;
                framebuffer[index + 2] = b;
            }
        }
    }
}

