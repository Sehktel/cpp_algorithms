#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// Структура для представления ребра графа
struct Edge
{
    int source;      // Начальная вершина
    int destination; // Конечная вершина
    int weight;      // Вес ребра
    
    // Оператор сравнения для сортировки
    bool operator<(const Edge& other) const
    {
        return weight < other.weight;
    }
};

// Класс для Union-Find (Disjoint Set Union) - структура данных для проверки циклов
class UnionFind
{
    std::vector<int> parent; // Родитель каждой вершины
    std::vector<int> rank;   // Ранг для оптимизации (ранг дерева)
    bool verbose;             // Режим детального вывода

public:
    // Конструктор: создаем структуру для V вершин
    UnionFind(int V, bool verboseMode = false)
    {
        parent.resize(V);
        rank.resize(V, 0);
        verbose = verboseMode;
        
        // Изначально каждая вершина - это отдельное множество
        for (int i = 0; i < V; i++)
        {
            parent[i] = i;
        }
    }
    
    // Найти корень множества, к которому принадлежит вершина (с path compression)
    int find(int x)
    {
        if (parent[x] != x)
        {
            // Path compression: делаем родителя корнем
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    // Объединить два множества (union by rank)
    bool unite(int x, int y)
    {
        int rootX = find(x);
        int rootY = find(y);
        
        // Если вершины уже в одном множестве, возвращаем false (создастся цикл)
        if (rootX == rootY)
        {
            if (verbose)
            {
                std::cout << "    Вершины " << x << " и " << y 
                          << " уже в одном множестве (корень: " << rootX 
                          << ") - пропускаем (создастся цикл)" << std::endl;
            }
            return false;
        }
        
        // Union by rank: меньшее дерево присоединяем к большему
        if (rank[rootX] < rank[rootY])
        {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY])
        {
            parent[rootY] = rootX;
        }
        else
        {
            // Ранги равны - выбираем произвольно и увеличиваем ранг
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        
        if (verbose)
        {
            std::cout << "    Объединили множества: " << x << " (корень: " << rootX 
                      << ") и " << y << " (корень: " << rootY << ")" << std::endl;
        }
        
        return true;
    }
    
    // Вывод текущего состояния Union-Find
    void printState(int numVertices)
    {
        std::cout << "Состояние Union-Find:" << std::endl;
        std::cout << "  Вершина -> Корень: ";
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << i << "->" << find(i);
            if (i < numVertices - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

// Класс для представления неориентированного взвешенного графа
class Graph
{
    int numVertices;              // Количество вершин
    std::vector<Edge> edges;      // Список всех рёбер
    bool verbose;                  // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        verbose = verboseMode;
    }

    // Добавление взвешенного ребра между вершинами u и v (неориентированный граф)
    void addEdge(int u, int v, int weight)
    {
        Edge e;
        e.source = u;
        e.destination = v;
        e.weight = weight;
        edges.push_back(e);
    }

    // Алгоритм Крускала для построения минимального остовного дерева (MST)
    void kruskal()
    {
        std::cout << "\n=== Алгоритм Крускала для построения Минимального Остовного Дерева (MST) ===" << std::endl;
        std::cout << "MST - это подграф, который:" << std::endl;
        std::cout << "  1. Включает все вершины" << std::endl;
        std::cout << "  2. Является деревом (связный, без циклов)" << std::endl;
        std::cout << "  3. Имеет минимальную сумму весов рёбер\n" << std::endl;
        
        // Шаг 1: Сортируем все рёбра по весу (по возрастанию)
        std::sort(edges.begin(), edges.end());
        
        if (verbose)
        {
            std::cout << "Отсортированные рёбра по весу:" << std::endl;
            for (size_t i = 0; i < edges.size(); i++)
            {
                std::cout << "  " << i + 1 << ". " << edges[i].source 
                          << " -- " << edges[i].destination 
                          << " (вес: " << edges[i].weight << ")" << std::endl;
            }
            std::cout << std::endl;
        }
        
        // Шаг 2: Инициализируем Union-Find структуру
        UnionFind uf(numVertices, verbose);
        
        std::vector<Edge> mstEdges; // Рёбра MST для финального вывода
        int totalWeight = 0;
        int step = 1;
        
        if (verbose)
        {
            uf.printState(numVertices);
        }
        
        // Шаг 3: Проходим по отсортированным рёбрам
        for (size_t i = 0; i < edges.size(); i++)
        {
            Edge e = edges[i];
            
            if (verbose)
            {
                std::cout << "\n--- Шаг " << step << " ---" << std::endl;
                std::cout << "Рассматриваем ребро: " << e.source 
                          << " -- " << e.destination 
                          << " (вес: " << e.weight << ")" << std::endl;
            }
            
            // Проверяем, не создаст ли это ребро цикл
            // Если вершины в разных множествах - добавляем ребро
            if (uf.unite(e.source, e.destination))
            {
                // Ребро добавлено в MST
                mstEdges.push_back(e);
                totalWeight += e.weight;
                
                if (verbose)
                {
                    std::cout << "  ✓ Добавили ребро в MST" << std::endl;
                    std::cout << "  Текущий вес MST: " << totalWeight << std::endl;
                    std::cout << "  Количество рёбер в MST: " << mstEdges.size() << std::endl;
                    uf.printState(numVertices);
                }
                
                // Если добавили V-1 рёбер, MST готово
                if ((int)mstEdges.size() == numVertices - 1)
                {
                    if (verbose)
                    {
                        std::cout << "\n✓ Добавлено V-1 рёбер, MST построено!" << std::endl;
                    }
                    break;
                }
            }
            
            step++;
        }
        
        // Выводим результаты
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
        std::cout << "Минимальное остовное дерево (MST):" << std::endl;
        std::cout << "Рёбра MST (в порядке добавления):" << std::endl;
        
        for (size_t i = 0; i < mstEdges.size(); i++)
        {
            std::cout << "  " << mstEdges[i].source << " -- " 
                      << mstEdges[i].destination 
                      << " (вес: " << mstEdges[i].weight << ")" << std::endl;
        }
        
        std::cout << "\nОбщий вес MST: " << totalWeight << std::endl;
        std::cout << "Количество рёбер в MST: " << mstEdges.size() << std::endl;
        std::cout << "Количество вершин: " << numVertices << std::endl;
        
        // Проверка корректности (в дереве должно быть V-1 рёбер)
        if ((int)mstEdges.size() == numVertices - 1)
        {
            std::cout << "✓ MST корректно построено (V-1 = " << (numVertices - 1) << " рёбер)" << std::endl;
        }
        else
        {
            std::cout << "⚠ ВНИМАНИЕ: Граф несвязный! MST неполное." << std::endl;
        }
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Простой связный граф" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 1: Простой связный граф
    //     0
    //    /|\
    //   1 4 2
    //    \ /
    //     2
    //     |
    //     3
    Graph g1(4, true);
    g1.addEdge(0, 1, 1);
    g1.addEdge(0, 2, 4);
    g1.addEdge(1, 2, 2);
    g1.addEdge(2, 3, 1);
    g1.kruskal();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Полный граф K4" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 2: Полный граф (все вершины соединены)
    Graph g2(4, true);
    g2.addEdge(0, 1, 10);
    g2.addEdge(0, 2, 6);
    g2.addEdge(0, 3, 5);
    g2.addEdge(1, 2, 15);
    g2.addEdge(1, 3, 4);
    g2.addEdge(2, 3, 8);
    g2.kruskal();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Линейная цепь" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 3: Линейная цепь (уже является деревом)
    // 0 --2-- 1 --3-- 2 --1-- 3 --4-- 4
    Graph g3(5, true);
    g3.addEdge(0, 1, 2);
    g3.addEdge(1, 2, 3);
    g3.addEdge(2, 3, 1);
    g3.addEdge(3, 4, 4);
    g3.kruskal();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 4: Сбалансированное дерево" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 4: Более сложный граф
    //         0
    //       / | \
    //      2  5  3
    //     /   |   \
    //    1    2    2
    //    |    |    |
    //    3    4    5
    //     \   |   /
    //      1  6  4
    //       \ | /
    //         6
    Graph g4(7, true);
    g4.addEdge(0, 1, 2);
    g4.addEdge(0, 2, 5);
    g4.addEdge(0, 3, 3);
    g4.addEdge(1, 4, 1);
    g4.addEdge(2, 4, 2);
    g4.addEdge(3, 5, 4);
    g4.addEdge(4, 6, 6);
    g4.addEdge(5, 6, 1);
    g4.kruskal();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 5: Большой граф (без детального вывода)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 5: Большой граф
    Graph g5(9, false);
    g5.addEdge(0, 1, 4);
    g5.addEdge(0, 7, 8);
    g5.addEdge(1, 2, 8);
    g5.addEdge(1, 7, 11);
    g5.addEdge(2, 3, 7);
    g5.addEdge(2, 5, 4);
    g5.addEdge(2, 8, 2);
    g5.addEdge(3, 4, 9);
    g5.addEdge(3, 5, 14);
    g5.addEdge(4, 5, 10);
    g5.addEdge(5, 6, 2);
    g5.addEdge(6, 7, 1);
    g5.addEdge(6, 8, 6);
    g5.addEdge(7, 8, 7);
    g5.kruskal();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Алгоритм Крускала:" << std::endl;
    std::cout << "- Находит минимальное остовное дерево (MST)" << std::endl;
    std::cout << "- Жадный алгоритм: сортирует рёбра и выбирает минимальные" << std::endl;
    std::cout << "- Использует Union-Find для проверки циклов" << std::endl;
    std::cout << "- Работает только с неориентированными связными графами" << std::endl;
    std::cout << "- Сложность: O(E log E) из-за сортировки" << std::endl;
    std::cout << "- Лучше для разреженных графов (E ≈ V)" << std::endl;
    std::cout << "- MST содержит V-1 рёбер для графа с V вершинами" << std::endl;
    std::cout << "- Применение: сети (электричество, вода), кластеризация, приближенные алгоритмы" << std::endl;
    
    return 0;
}

