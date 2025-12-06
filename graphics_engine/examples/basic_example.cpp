#include "../core/renderer/framebuffer.h"
#include "../rasterization/bresenham/line.h"
#include "../rasterization/bresenham/circle.h"
#include <iostream>

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Базовый пример графического движка" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Создаем framebuffer
    int width = 800;
    int height = 600;
    Framebuffer fb(width, height);
    
    // Очищаем экран черным
    fb.clear(0, 0, 0);
    
    std::cout << "\n--- Рисуем линии алгоритмом Брезенхема ---" << std::endl;
    
    // Рисуем несколько линий
    BresenhamLine::draw(100, 100, 700, 100, fb.getData(), width, height, 255, 0, 0);
    BresenhamLine::draw(100, 200, 700, 200, fb.getData(), width, height, 0, 255, 0);
    BresenhamLine::draw(100, 300, 700, 300, fb.getData(), width, height, 0, 0, 255);
    
    std::cout << "\n--- Рисуем окружности ---" << std::endl;
    
    // Рисуем окружности
    BresenhamCircle::draw(200, 400, 50, fb.getData(), width, height, 255, 255, 0);
    BresenhamCircle::draw(400, 400, 50, fb.getData(), width, height, 255, 0, 255);
    BresenhamCircle::draw(600, 400, 50, fb.getData(), width, height, 0, 255, 255);
    
    // Рисуем заполненные окружности
    BresenhamCircle::drawFilled(200, 500, 30, fb.getData(), width, height, 255, 128, 0);
    BresenhamCircle::drawFilled(400, 500, 30, fb.getData(), width, height, 128, 255, 0);
    BresenhamCircle::drawFilled(600, 500, 30, fb.getData(), width, height, 0, 128, 255);
    
    // Сохраняем результат
    std::cout << "\n--- Сохраняем изображение ---" << std::endl;
    if (fb.saveToBMP("output.bmp"))
    {
        std::cout << "Изображение сохранено в output.bmp" << std::endl;
    }
    else
    {
        std::cout << "Ошибка при сохранении изображения" << std::endl;
    }
    
    return 0;
}

