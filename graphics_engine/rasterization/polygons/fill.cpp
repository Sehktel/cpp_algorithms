#include "fill.h"
#include <algorithm>
#include <vector>
#include <queue>

struct Edge
{
    int yMax;
    float x;
    float dx;
    
    Edge(int yMax, float x, float dx) : yMax(yMax), x(x), dx(dx) {}
};

void PolygonFiller::scanlineFill(int* points, int numPoints,
                                 unsigned char* framebuffer, int width, int height,
                                 unsigned char r, unsigned char g, unsigned char b)
{
    if (numPoints < 3)
        return;
    
    // Находим границы полигона
    int yMin = points[1];
    int yMax = points[1];
    for (int i = 0; i < numPoints; i += 2)
    {
        if (points[i + 1] < yMin) yMin = points[i + 1];
        if (points[i + 1] > yMax) yMax = points[i + 1];
    }
    
    // Строим таблицу активных ребер
    std::vector<std::vector<Edge>> edgeTable(yMax - yMin + 1);
    
    int numVertices = numPoints / 2;
    for (int i = 0; i < numVertices; i++)
    {
        int j = (i + 1) % numVertices;
        int x0 = points[i * 2];
        int y0 = points[i * 2 + 1];
        int x1 = points[j * 2];
        int y1 = points[j * 2 + 1];
        
        if (y0 == y1)
            continue;  // Горизонтальное ребро пропускаем
        
        int yMinEdge = (y0 < y1) ? y0 : y1;
        int yMaxEdge = (y0 > y1) ? y0 : y1;
        
        float dx = (float)(x1 - x0) / (y1 - y0);
        float x = (yMinEdge == y0) ? x0 : x1;
        
        if (yMinEdge - yMin >= 0 && yMinEdge - yMin < edgeTable.size())
        {
            edgeTable[yMinEdge - yMin].push_back(Edge(yMaxEdge, x, dx));
        }
    }
    
    // Заполняем полигон построчно
    std::vector<Edge> activeEdges;
    
    for (int y = yMin; y <= yMax; y++)
    {
        // Добавляем новые ребра
        if (y - yMin < edgeTable.size())
        {
            for (const auto& edge : edgeTable[y - yMin])
            {
                activeEdges.push_back(edge);
            }
        }
        
        // Удаляем завершившиеся ребра
        activeEdges.erase(
            std::remove_if(activeEdges.begin(), activeEdges.end(),
                [y](const Edge& e) { return e.yMax <= y; }),
            activeEdges.end()
        );
        
        // Сортируем по x
        std::sort(activeEdges.begin(), activeEdges.end(),
            [](const Edge& a, const Edge& b) { return a.x < b.x; });
        
        // Рисуем горизонтальные линии между парами ребер
        for (size_t i = 0; i < activeEdges.size(); i += 2)
        {
            if (i + 1 < activeEdges.size())
            {
                int xStart = (int)activeEdges[i].x;
                int xEnd = (int)activeEdges[i + 1].x;
                
                for (int x = xStart; x <= xEnd; x++)
                {
                    if (x >= 0 && x < width && y >= 0 && y < height)
                    {
                        int index = (y * width + x) * 3;
                        framebuffer[index] = r;
                        framebuffer[index + 1] = g;
                        framebuffer[index + 2] = b;
                    }
                }
            }
        }
        
        // Обновляем x координаты активных ребер
        for (auto& edge : activeEdges)
        {
            edge.x += edge.dx;
        }
    }
}

void PolygonFiller::floodFill(int startX, int startY,
                              unsigned char* framebuffer, int width, int height,
                              unsigned char r, unsigned char g, unsigned char b,
                              unsigned char targetR, unsigned char targetG, unsigned char targetB)
{
    std::queue<std::pair<int, int>> queue;
    queue.push({startX, startY});
    
    while (!queue.empty())
    {
        auto [x, y] = queue.front();
        queue.pop();
        
        if (x < 0 || x >= width || y < 0 || y >= height)
            continue;
        
        int index = (y * width + x) * 3;
        unsigned char currentR = framebuffer[index];
        unsigned char currentG = framebuffer[index + 1];
        unsigned char currentB = framebuffer[index + 2];
        
        if (currentR == targetR && currentG == targetG && currentB == targetB)
            continue;
        
        if (currentR == r && currentG == g && currentB == b)
            continue;
        
        framebuffer[index] = r;
        framebuffer[index + 1] = g;
        framebuffer[index + 2] = b;
        
        queue.push({x + 1, y});
        queue.push({x - 1, y});
        queue.push({x, y + 1});
        queue.push({x, y - 1});
    }
}

