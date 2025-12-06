#include "line.h"
#include <cmath>
#include <algorithm>

void BresenhamLine::draw(int x0, int y0, int x1, int y1, 
                        void (*callback)(int x, int y))
{
    // Алгоритм Брезенхема для линий
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0;
    int y = y0;
    
    while (true)
    {
        callback(x, y);
        
        if (x == x1 && y == y1)
            break;
        
        int e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

void BresenhamLine::draw(int x0, int y0, int x1, int y1,
                         unsigned char* framebuffer, int width, int height,
                         unsigned char r, unsigned char g, unsigned char b)
{
    draw(x0, y0, x1, y1, [&](int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            int index = (y * width + x) * 3;
            framebuffer[index] = r;
            framebuffer[index + 1] = g;
            framebuffer[index + 2] = b;
        }
    });
}

