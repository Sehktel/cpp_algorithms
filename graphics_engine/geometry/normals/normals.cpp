#include "normals.h"
#include "../../core/math/vector3.h"
#include <cmath>

void Normals::calculateTriangleNormal(const float* v0, const float* v1, const float* v2, float* normal)
{
    // Векторы двух сторон треугольника
    Vector3 edge1(v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]);
    Vector3 edge2(v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]);
    
    // Векторное произведение дает нормаль
    Vector3 n = edge1.cross(edge2);
    n = n.normalize();
    
    normal[0] = n.x;
    normal[1] = n.y;
    normal[2] = n.z;
}

void Normals::calculatePolygonNormal(const float* vertices, int numVertices, float* normal)
{
    if (numVertices < 3)
    {
        normal[0] = 0;
        normal[1] = 0;
        normal[2] = 1;
        return;
    }
    
    // Используем метод Newell для вычисления нормали многоугольника
    float nx = 0, ny = 0, nz = 0;
    
    for (int i = 0; i < numVertices; i++)
    {
        int j = (i + 1) % numVertices;
        
        float xi = vertices[i * 3];
        float yi = vertices[i * 3 + 1];
        float zi = vertices[i * 3 + 2];
        
        float xj = vertices[j * 3];
        float yj = vertices[j * 3 + 1];
        float zj = vertices[j * 3 + 2];
        
        nx += (yi - yj) * (zi + zj);
        ny += (zi - zj) * (xi + xj);
        nz += (xi - xj) * (yi + yj);
    }
    
    // Нормализуем
    float len = std::sqrt(nx*nx + ny*ny + nz*nz);
    if (len > 0.0001f)
    {
        normal[0] = nx / len;
        normal[1] = ny / len;
        normal[2] = nz / len;
    }
    else
    {
        normal[0] = 0;
        normal[1] = 0;
        normal[2] = 1;
    }
}

void Normals::calculateMeshNormals(const float* vertices, int numVertices,
                                   const unsigned int* indices, int numIndices,
                                   float* outNormals)
{
    // Инициализируем нормали нулями
    for (int i = 0; i < numVertices; i++)
    {
        outNormals[i * 3] = 0;
        outNormals[i * 3 + 1] = 0;
        outNormals[i * 3 + 2] = 0;
    }
    
    // Вычисляем нормали для каждого треугольника
    int numTriangles = numIndices / 3;
    for (int i = 0; i < numTriangles; i++)
    {
        unsigned int i0 = indices[i * 3];
        unsigned int i1 = indices[i * 3 + 1];
        unsigned int i2 = indices[i * 3 + 2];
        
        const float* v0 = &vertices[i0 * 3];
        const float* v1 = &vertices[i1 * 3];
        const float* v2 = &vertices[i2 * 3];
        
        float faceNormal[3];
        calculateTriangleNormal(v0, v1, v2, faceNormal);
        
        // Добавляем нормаль грани к нормалям вершин
        outNormals[i0 * 3] += faceNormal[0];
        outNormals[i0 * 3 + 1] += faceNormal[1];
        outNormals[i0 * 3 + 2] += faceNormal[2];
        
        outNormals[i1 * 3] += faceNormal[0];
        outNormals[i1 * 3 + 1] += faceNormal[1];
        outNormals[i1 * 3 + 2] += faceNormal[2];
        
        outNormals[i2 * 3] += faceNormal[0];
        outNormals[i2 * 3 + 1] += faceNormal[1];
        outNormals[i2 * 3 + 2] += faceNormal[2];
    }
    
    // Нормализуем все нормали вершин
    for (int i = 0; i < numVertices; i++)
    {
        normalize(&outNormals[i * 3]);
    }
}

void Normals::calculateVertexNormal(const float* vertex, const float* adjacentVertices, 
                                   int numAdjacent, float* normal)
{
    if (numAdjacent < 2)
    {
        normal[0] = 0;
        normal[1] = 0;
        normal[2] = 1;
        return;
    }
    
    float nx = 0, ny = 0, nz = 0;
    
    // Вычисляем нормали для всех треугольников, образованных вершиной и соседними
    for (int i = 0; i < numAdjacent; i++)
    {
        int j = (i + 1) % numAdjacent;
        
        const float* v1 = &adjacentVertices[i * 3];
        const float* v2 = &adjacentVertices[j * 3];
        
        float faceNormal[3];
        calculateTriangleNormal(vertex, v1, v2, faceNormal);
        
        nx += faceNormal[0];
        ny += faceNormal[1];
        nz += faceNormal[2];
    }
    
    normal[0] = nx;
    normal[1] = ny;
    normal[2] = nz;
    normalize(normal);
}

void Normals::normalize(float* vector)
{
    float len = std::sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    if (len > 0.0001f)
    {
        vector[0] /= len;
        vector[1] /= len;
        vector[2] /= len;
    }
    else
    {
        vector[0] = 0;
        vector[1] = 0;
        vector[2] = 1;
    }
}

