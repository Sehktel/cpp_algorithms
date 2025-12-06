#pragma once

// Расчет нормалей для геометрии
class Normals
{
public:
    // Вычисляет нормаль треугольника по трем вершинам
    static void calculateTriangleNormal(const float* v0, const float* v1, const float* v2, float* normal);
    
    // Вычисляет нормаль полигона (многоугольника)
    static void calculatePolygonNormal(const float* vertices, int numVertices, float* normal);
    
    // Вычисляет нормали для всех треугольников в меше
    // vertices - массив вершин (x,y,z), numVertices - количество вершин
    // indices - массив индексов треугольников, numIndices - количество индексов
    // outNormals - выходной массив нормалей (по одной на вершину)
    static void calculateMeshNormals(const float* vertices, int numVertices,
                                     const unsigned int* indices, int numIndices,
                                     float* outNormals);
    
    // Вычисляет нормаль вершины как среднее нормалей соседних граней
    static void calculateVertexNormal(const float* vertex, const float* adjacentVertices, 
                                     int numAdjacent, float* normal);
    
    // Нормализует вектор
    static void normalize(float* vector);
};

