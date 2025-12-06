#pragma once

// Структура для хранения 3D меша
struct Mesh
{
    float* vertices;        // Массив вершин (x,y,z)
    float* normals;         // Массив нормалей (x,y,z)
    float* texCoords;       // Массив текстурных координат (u,v)
    unsigned int* indices;  // Массив индексов треугольников
    int numVertices;
    int numIndices;
    
    Mesh() : vertices(nullptr), normals(nullptr), texCoords(nullptr), 
             indices(nullptr), numVertices(0), numIndices(0) {}
    
    ~Mesh()
    {
        delete[] vertices;
        delete[] normals;
        delete[] texCoords;
        delete[] indices;
    }
};

// Генераторы 3D геометрии
class MeshGenerator
{
public:
    // Генерирует тор (тороид)
    // majorRadius - большой радиус тора
    // minorRadius - малый радиус тора
    // majorSegments - количество сегментов по большому радиусу
    // minorSegments - количество сегментов по малому радиусу
    static Mesh generateTorus(float majorRadius, float minorRadius, 
                             int majorSegments, int minorSegments);
    
    // Генерирует сферу
    static Mesh generateSphere(float radius, int segments, int rings);
    
    // Генерирует куб
    static Mesh generateCube(float size);
    
    // Генерирует цилиндр
    static Mesh generateCylinder(float radius, float height, int segments);
    
    // Генерирует конус
    static Mesh generateCone(float radius, float height, int segments);
    
    // Генерирует плоскость (квадрат)
    static Mesh generatePlane(float width, float height, int segmentsX, int segmentsY);
};

