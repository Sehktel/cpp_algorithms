#include "../core/renderer/framebuffer.h"
#include "../geometry/generators/mesh_generator.h"
#include "../core/math/matrix4.h"
#include "../core/camera/camera.h"
#include "../geometry/transforms/affine.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Проекция 3D точки на 2D экран
void projectPoint(const float* point3D, const Matrix4& viewProj, 
                  int screenWidth, int screenHeight,
                  int& screenX, int& screenY, float& depth)
{
    float vec[4];
    viewProj.multiplyPoint(point3D, vec);
    
    // Перспективное деление
    if (std::abs(vec[3]) > 0.0001f)
    {
        vec[0] /= vec[3];
        vec[1] /= vec[3];
        vec[2] /= vec[3];
    }
    
    // Преобразуем в координаты экрана
    screenX = (int)((vec[0] + 1.0f) * 0.5f * screenWidth);
    screenY = (int)((1.0f - vec[1]) * 0.5f * screenHeight);
    depth = vec[2];
}

// Рисует треугольник (wireframe)
void drawTriangle(const float* v0, const float* v1, const float* v2,
                  const Matrix4& viewProj,
                  unsigned char* framebuffer, int width, int height,
                  unsigned char r, unsigned char g, unsigned char b)
{
    int x0, y0, x1, y1, x2, y2;
    float d0, d1, d2;
    
    projectPoint(v0, viewProj, width, height, x0, y0, d0);
    projectPoint(v1, viewProj, width, height, x1, y1, d1);
    projectPoint(v2, viewProj, width, height, x2, y2, d2);
    
    // Рисуем линии треугольника
    auto drawLine = [&](int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        int x = x0;
        int y = y0;
        
        while (true)
        {
            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                int index = (y * width + x) * 3;
                framebuffer[index] = r;
                framebuffer[index + 1] = g;
                framebuffer[index + 2] = b;
            }
            
            if (x == x1 && y == y1)
                break;
            
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x += sx; }
            if (e2 < dx) { err += dx; y += sy; }
        }
    };
    
    drawLine(x0, y0, x1, y1);
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x0, y0);
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Пример: Генерация и отображение тора" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Создаем framebuffer
    int width = 800;
    int height = 600;
    Framebuffer fb(width, height);
    fb.clear(20, 20, 30);  // Темно-синий фон
    
    std::cout << "\n--- Генерируем тор ---" << std::endl;
    Mesh torus = MeshGenerator::generateTorus(2.0f, 0.5f, 32, 16);
    std::cout << "Вершин: " << torus.numVertices << std::endl;
    std::cout << "Индексов: " << torus.numIndices << std::endl;
    std::cout << "Треугольников: " << torus.numIndices / 3 << std::endl;
    
    std::cout << "\n--- Настраиваем камеру ---" << std::endl;
    Camera camera;
    camera.setPosition(0, 0, 8);
    camera.setTarget(0, 0, 0);
    camera.setUp(0, 1, 0);
    camera.setProjection(45.0f, (float)width / height, 0.1f, 100.0f);
    
    Matrix4 viewMatrix = camera.getViewMatrix();
    Matrix4 projMatrix = camera.getProjectionMatrix();
    Matrix4 viewProj = projMatrix * viewMatrix;
    
    // Поворачиваем тор
    float angle = M_PI / 4.0f;  // 45 градусов
    Matrix4 rotation = Matrix4::rotationY(angle) * Matrix4::rotationX(angle * 0.5f);
    
    std::cout << "\n--- Рендерим тор (wireframe) ---" << std::endl;
    
    // Преобразуем вершины и рисуем треугольники
    for (int i = 0; i < torus.numIndices; i += 3)
    {
        unsigned int i0 = torus.indices[i];
        unsigned int i1 = torus.indices[i + 1];
        unsigned int i2 = torus.indices[i + 2];
        
        const float* v0 = &torus.vertices[i0 * 3];
        const float* v1 = &torus.vertices[i1 * 3];
        const float* v2 = &torus.vertices[i2 * 3];
        
        // Применяем поворот
        float v0Final[3], v1Final[3], v2Final[3];
        rotation.multiplyPoint(v0, v0Final);
        rotation.multiplyPoint(v1, v1Final);
        rotation.multiplyPoint(v2, v2Final);
        
        // Рисуем треугольник
        drawTriangle(v0Final, v1Final, v2Final,
                   viewProj, fb.getData(), width, height,
                   200, 150, 100);  // Оранжевый цвет
    }
    
    // Сохраняем результат
    std::cout << "\n--- Сохраняем изображение ---" << std::endl;
    if (fb.saveToBMP("torus.bmp"))
    {
        std::cout << "Изображение сохранено в torus.bmp" << std::endl;
    }
    else
    {
        std::cout << "Ошибка при сохранении изображения" << std::endl;
    }
    
    std::cout << "\n=== ВЫВОД ===" << std::endl;
    std::cout << "Тор успешно сгенерирован и отображен!" << std::endl;
    std::cout << "- Большой радиус: 2.0" << std::endl;
    std::cout << "- Малый радиус: 0.5" << std::endl;
    std::cout << "- Сегментов по большому радиусу: 32" << std::endl;
    std::cout << "- Сегментов по малому радиусу: 16" << std::endl;
    
    return 0;
}

