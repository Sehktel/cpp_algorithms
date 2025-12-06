#include "mesh_generator.h"
#include "../normals/normals.h"
#include <cmath>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mesh MeshGenerator::generateTorus(float majorRadius, float minorRadius, 
                                 int majorSegments, int minorSegments)
{
    Mesh mesh;
    
    // Вычисляем количество вершин и индексов
    int numVertices = (majorSegments + 1) * (minorSegments + 1);
    int numIndices = majorSegments * minorSegments * 6;
    
    mesh.vertices = new float[numVertices * 3];
    mesh.normals = new float[numVertices * 3];
    mesh.texCoords = new float[numVertices * 2];
    mesh.indices = new unsigned int[numIndices];
    mesh.numVertices = numVertices;
    mesh.numIndices = numIndices;
    
    // Генерируем вершины
    int vertexIndex = 0;
    for (int i = 0; i <= majorSegments; i++)
    {
        float u = (float)i / majorSegments * 2.0f * M_PI;
        float cosU = std::cos(u);
        float sinU = std::sin(u);
        
        for (int j = 0; j <= minorSegments; j++)
        {
            float v = (float)j / minorSegments * 2.0f * M_PI;
            float cosV = std::cos(v);
            float sinV = std::sin(v);
            
            // Позиция вершины
            float x = (majorRadius + minorRadius * cosV) * cosU;
            float y = (majorRadius + minorRadius * cosV) * sinU;
            float z = minorRadius * sinV;
            
            mesh.vertices[vertexIndex * 3] = x;
            mesh.vertices[vertexIndex * 3 + 1] = y;
            mesh.vertices[vertexIndex * 3 + 2] = z;
            
            // Нормаль (направлена от центра тора)
            float nx = cosV * cosU;
            float ny = cosV * sinU;
            float nz = sinV;
            
            mesh.normals[vertexIndex * 3] = nx;
            mesh.normals[vertexIndex * 3 + 1] = ny;
            mesh.normals[vertexIndex * 3 + 2] = nz;
            
            // Текстурные координаты
            mesh.texCoords[vertexIndex * 2] = (float)i / majorSegments;
            mesh.texCoords[vertexIndex * 2 + 1] = (float)j / minorSegments;
            
            vertexIndex++;
        }
    }
    
    // Генерируем индексы
    int indexIndex = 0;
    for (int i = 0; i < majorSegments; i++)
    {
        for (int j = 0; j < minorSegments; j++)
        {
            int current = i * (minorSegments + 1) + j;
            int next = current + minorSegments + 1;
            
            // Первый треугольник
            mesh.indices[indexIndex++] = current;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = current + 1;
            
            // Второй треугольник
            mesh.indices[indexIndex++] = current + 1;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = next + 1;
        }
    }
    
    return mesh;
}

Mesh MeshGenerator::generateSphere(float radius, int segments, int rings)
{
    Mesh mesh;
    
    int numVertices = (segments + 1) * (rings + 1);
    int numIndices = segments * rings * 6;
    
    mesh.vertices = new float[numVertices * 3];
    mesh.normals = new float[numVertices * 3];
    mesh.texCoords = new float[numVertices * 2];
    mesh.indices = new unsigned int[numIndices];
    mesh.numVertices = numVertices;
    mesh.numIndices = numIndices;
    
    // Генерируем вершины
    int vertexIndex = 0;
    for (int i = 0; i <= rings; i++)
    {
        float theta = (float)i / rings * M_PI;  // От 0 до π
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);
        
        for (int j = 0; j <= segments; j++)
        {
            float phi = (float)j / segments * 2.0f * M_PI;  // От 0 до 2π
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            
            // Позиция вершины
            float x = radius * sinTheta * cosPhi;
            float y = radius * cosTheta;
            float z = radius * sinTheta * sinPhi;
            
            mesh.vertices[vertexIndex * 3] = x;
            mesh.vertices[vertexIndex * 3 + 1] = y;
            mesh.vertices[vertexIndex * 3 + 2] = z;
            
            // Нормаль (направлена от центра сферы)
            mesh.normals[vertexIndex * 3] = x / radius;
            mesh.normals[vertexIndex * 3 + 1] = y / radius;
            mesh.normals[vertexIndex * 3 + 2] = z / radius;
            
            // Текстурные координаты
            mesh.texCoords[vertexIndex * 2] = (float)j / segments;
            mesh.texCoords[vertexIndex * 2 + 1] = (float)i / rings;
            
            vertexIndex++;
        }
    }
    
    // Генерируем индексы
    int indexIndex = 0;
    for (int i = 0; i < rings; i++)
    {
        for (int j = 0; j < segments; j++)
        {
            int current = i * (segments + 1) + j;
            int next = current + segments + 1;
            
            // Первый треугольник
            mesh.indices[indexIndex++] = current;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = current + 1;
            
            // Второй треугольник
            mesh.indices[indexIndex++] = current + 1;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = next + 1;
        }
    }
    
    return mesh;
}

Mesh MeshGenerator::generateCube(float size)
{
    Mesh mesh;
    
    float half = size / 2.0f;
    
    // 8 вершин куба
    mesh.numVertices = 24;  // 6 граней * 4 вершины
    mesh.numIndices = 36;   // 6 граней * 2 треугольника * 3 индекса
    
    mesh.vertices = new float[mesh.numVertices * 3];
    mesh.normals = new float[mesh.numVertices * 3];
    mesh.texCoords = new float[mesh.numVertices * 2];
    mesh.indices = new unsigned int[mesh.numIndices];
    
    // Вершины куба (каждая грань имеет 4 вершины)
    float vertices[] = {
        // Передняя грань
        -half, -half,  half,   half, -half,  half,   half,  half,  half,  -half,  half,  half,
        // Задняя грань
         half, -half, -half,  -half, -half, -half,  -half,  half, -half,   half,  half, -half,
        // Верхняя грань
        -half,  half,  half,   half,  half,  half,   half,  half, -half,  -half,  half, -half,
        // Нижняя грань
        -half, -half, -half,   half, -half, -half,   half, -half,  half,  -half, -half,  half,
        // Правая грань
         half, -half,  half,   half, -half, -half,   half,  half, -half,   half,  half,  half,
        // Левая грань
        -half, -half, -half,  -half, -half,  half,  -half,  half,  half,  -half,  half, -half
    };
    
    float normals[] = {
        // Передняя
        0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,
        // Задняя
        0, 0, -1,  0, 0, -1,  0, 0, -1,  0, 0, -1,
        // Верхняя
        0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,
        // Нижняя
        0, -1, 0,  0, -1, 0,  0, -1, 0,  0, -1, 0,
        // Правая
        1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,
        // Левая
        -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0
    };
    
    float texCoords[] = {
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1,
        0, 0,  1, 0,  1, 1,  0, 1
    };
    
    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,      // Передняя
        4, 5, 6,  6, 7, 4,      // Задняя
        8, 9, 10, 10, 11, 8,    // Верхняя
        12, 13, 14, 14, 15, 12, // Нижняя
        16, 17, 18, 18, 19, 16, // Правая
        20, 21, 22, 22, 23, 20  // Левая
    };
    
    std::memcpy(mesh.vertices, vertices, sizeof(vertices));
    std::memcpy(mesh.normals, normals, sizeof(normals));
    std::memcpy(mesh.texCoords, texCoords, sizeof(texCoords));
    std::memcpy(mesh.indices, indices, sizeof(indices));
    
    return mesh;
}

Mesh MeshGenerator::generateCylinder(float radius, float height, int segments)
{
    Mesh mesh;
    
    int numVertices = (segments + 1) * 2 + segments * 2;  // Верх, низ, бок
    int numIndices = segments * 12;  // Верх, низ, бок
    
    mesh.vertices = new float[numVertices * 3];
    mesh.normals = new float[numVertices * 3];
    mesh.texCoords = new float[numVertices * 2];
    mesh.indices = new unsigned int[numIndices];
    mesh.numVertices = numVertices;
    mesh.numIndices = numIndices;
    
    // TODO: Реализовать генерацию цилиндра
    // Аналогично тору, но с плоскими крышками
    
    return mesh;
}

Mesh MeshGenerator::generateCone(float radius, float height, int segments)
{
    Mesh mesh;
    // TODO: Реализовать генерацию конуса
    return mesh;
}

Mesh MeshGenerator::generatePlane(float width, float height, int segmentsX, int segmentsY)
{
    Mesh mesh;
    // TODO: Реализовать генерацию плоскости
    return mesh;
}

