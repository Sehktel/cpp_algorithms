#include "phong.h"
#include "../core/math/vector3.h"
#include <cmath>

void PhongLighting::calculate(const float* position,
                              const float* normal,
                              const float* viewPos,
                              const Light& light,
                              const Material& material,
                              float* outColor)
{
    // Вектор от точки к свету
    Vector3 lightDir(
        light.position[0] - position[0],
        light.position[1] - position[1],
        light.position[2] - position[2]
    );
    lightDir = lightDir.normalize();
    
    // Нормаль
    Vector3 n(normal[0], normal[1], normal[2]);
    n = n.normalize();
    
    // Вектор от точки к камере
    Vector3 viewDir(
        viewPos[0] - position[0],
        viewPos[1] - position[1],
        viewPos[2] - position[2]
    );
    viewDir = viewDir.normalize();
    
    // Отраженный вектор
    Vector3 reflectDir = n * (2.0f * n.dot(lightDir)) - lightDir;
    reflectDir = reflectDir.normalize();
    
    // Окружающая компонента
    float ambientStrength = 0.1f;
    outColor[0] = material.ambient[0] * ambientStrength;
    outColor[1] = material.ambient[1] * ambientStrength;
    outColor[2] = material.ambient[2] * ambientStrength;
    
    // Диффузная компонента
    float diff = std::max(0.0f, n.dot(lightDir));
    outColor[0] += material.diffuse[0] * light.color[0] * diff * light.intensity;
    outColor[1] += material.diffuse[1] * light.color[1] * diff * light.intensity;
    outColor[2] += material.diffuse[2] * light.color[2] * diff * light.intensity;
    
    // Зеркальная компонента
    float spec = std::pow(std::max(0.0f, viewDir.dot(reflectDir)), material.shininess);
    outColor[0] += material.specular[0] * light.color[0] * spec * light.intensity;
    outColor[1] += material.specular[1] * light.color[1] * spec * light.intensity;
    outColor[2] += material.specular[2] * light.color[2] * spec * light.intensity;
    
    // Ограничиваем значения
    outColor[0] = std::min(1.0f, outColor[0]);
    outColor[1] = std::min(1.0f, outColor[1]);
    outColor[2] = std::min(1.0f, outColor[2]);
}

