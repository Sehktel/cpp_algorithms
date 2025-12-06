#pragma once

struct Vector3;

// Модель освещения Фонга
class PhongLighting
{
public:
    struct Light
    {
        float position[3];
        float color[3];  // RGB
        float intensity;
    };
    
    struct Material
    {
        float ambient[3];   // Окружающий цвет
        float diffuse[3];   // Диффузный цвет
        float specular[3];  // Зеркальный цвет
        float shininess;    // Блеск
    };
    
    // Вычисляет цвет точки по модели Фонга
    static void calculate(const float* position,      // Позиция точки
                         const float* normal,         // Нормаль
                         const float* viewPos,        // Позиция камеры
                         const Light& light,
                         const Material& material,
                         float* outColor);            // Результирующий цвет [R, G, B]
};

