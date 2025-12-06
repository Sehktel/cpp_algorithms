#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>

// Структура для представления ребра графа
struct Edge
{
    int source;      // Начальная вершина
    int destination; // Конечная вершина
    int weight;      // Вес ребра
};

// Класс для Union-Find (Disjoint Set Union) - структура данных для компонентов связности
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
        
        // Если вершины уже в одном множестве, возвращаем false
        if (rootX == rootY)
        {
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
        
        return true;
    }
    
    // Проверить, находятся ли вершины в одном множестве
    bool same(int x, int y)
    {
        return find(x) == find(y);
    }
    
    // Получить количество компонентов связности
    int getComponentCount(int numVertices)
    {
        std::vector<bool> roots(numVertices, false);
        int count = 0;
        for (int i = 0; i < numVertices; i++)
        {
            int root = find(i);
            if (!roots[root])
            {
                roots[root] = true;
                count++;
            }
        }
        return count;
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
        
        // Группировка по компонентам
        std::vector<std::vector<int>> components(numVertices);
        for (int i = 0; i < numVertices; i++)
        {
            components[find(i)].push_back(i);
        }
        
        std::cout << "  Компоненты связности:" << std::endl;
        for (int i = 0; i < numVertices; i++)
        {
            if (!components[i].empty())
            {
                std::cout << "    Компонент " << i << ": [";
                for (size_t j = 0; j < components[i].size(); j++)
                {
                    std::cout << components[i][j];
                    if (j < components[i].size() - 1) std::cout << ", ";
                }
                std::cout << "]" << std::endl;
            }
        }
    }
};

// Класс для представления неориентированного взвешенного графа
class Graph
{
    int numVertices;              // Количество вершин
    std::vector<Edge> edges;     // Список всех рёбер
    std::vector<std::vector<Edge>> adj; // Список смежности (для поиска минимальных рёбер)
    bool verbose;                 // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        verbose = verboseMode;
        adj.resize(V);
    }

    // Добавление взвешенного ребра между вершинами u и v (неориентированный граф)
    void addEdge(int u, int v, int weight)
    {
        Edge e1, e2;
        e1.source = u;
        e1.destination = v;
        e1.weight = weight;
        
        e2.source = v;
        e2.destination = u;
        e2.weight = weight;
        
        edges.push_back(e1);
        adj[u].push_back(e2); // Для вершины u храним ребро к v
        adj[v].push_back(e1); // Для вершины v храним ребро к u
    }

    // Алгоритм Борувки для построения минимального остовного дерева (MST)
    void boruvka()
    {
        std::cout << "\n=== Алгоритм Борувки для построения Минимального Остовного Дерева (MST) ===" << std::endl;
        std::cout << "MST - это подграф, который:" << std::endl;
        std::cout << "  1. Включает все вершины" << std::endl;
        std::cout << "  2. Является деревом (связный, без циклов)" << std::endl;
        std::cout << "  3. Имеет минимальную сумму весов рёбер\n" << std::endl;
        std::cout << "Алгоритм Борувки работает итеративно:" << std::endl;
        std::cout << "  - На каждой итерации находит минимальное ребро для каждого компонента" << std::endl;
        std::cout << "  - Добавляет эти рёбра в MST" << std::endl;
        std::cout << "  - Объединяет компоненты" << std::endl;
        std::cout << "  - Повторяет, пока не останется один компонент\n" << std::endl;
        
        // Инициализируем Union-Find структуру
        UnionFind uf(numVertices, verbose);
        
        std::vector<Edge> mstEdges; // Рёбра MST для финального вывода
        int totalWeight = 0;
        int iteration = 1;
        
        if (verbose)
        {
            uf.printState(numVertices);
        }
        
        // Основной цикл: повторяем, пока не останется один компонент
        while (uf.getComponentCount(numVertices) > 1)
        {
            if (verbose)
            {
                std::cout << "\n=== ИТЕРАЦИЯ " << iteration << " ===" << std::endl;
                std::cout << "Количество компонентов: " << uf.getComponentCount(numVertices) << std::endl;
            }
            
            // Для каждого компонента находим минимальное ребро, выходящее из него
            // cheapest[i] = индекс минимального ребра для компонента с корнем i
            std::vector<int> cheapest(numVertices, -1);
            
            // Проходим по всем рёбрам и находим минимальное для каждого компонента
            for (size_t i = 0; i < edges.size(); i++)
            {
                Edge e = edges[i];
                int rootU = uf.find(e.source);
                int rootV = uf.find(e.destination);
                
                // Если ребро соединяет разные компоненты
                if (rootU != rootV)
                {
                    // Обновляем минимальное ребро для компонента rootU
                    if (cheapest[rootU] == -1 || edges[cheapest[rootU]].weight > e.weight)
                    {
                        cheapest[rootU] = i;
                    }
                    
                    // Обновляем минимальное ребро для компонента rootV
                    if (cheapest[rootV] == -1 || edges[cheapest[rootV]].weight > e.weight)
                    {
                        cheapest[rootV] = i;
                    }
                }
            }
            
            if (verbose)
            {
                std::cout << "Найденные минимальные рёбра для компонентов:" << std::endl;
                for (int i = 0; i < numVertices; i++)
                {
                    if (cheapest[i] != -1 && uf.find(i) == i) // Только для корней компонентов
                    {
                        Edge e = edges[cheapest[i]];
                        std::cout << "  Компонент " << i << ": ребро " 
                                  << e.source << " -- " << e.destination 
                                  << " (вес: " << e.weight << ")" << std::endl;
                    }
                }
            }
            
            // Добавляем найденные минимальные рёбра в MST
            bool addedAny = false;
            for (int i = 0; i < numVertices; i++)
            {
                if (cheapest[i] != -1 && uf.find(i) == i) // Только для корней компонентов
                {
                    Edge e = edges[cheapest[i]];
                    int rootU = uf.find(e.source);
                    int rootV = uf.find(e.destination);
                    
                    // Проверяем, что компоненты ещё разные (могли объединиться на этой итерации)
                    if (rootU != rootV)
                    {
                        // Добавляем ребро в MST
                        mstEdges.push_back(e);
                        totalWeight += e.weight;
                        uf.unite(rootU, rootV);
                        addedAny = true;
                        
                        if (verbose)
                        {
                            std::cout << "  ✓ Добавили ребро: " << e.source 
                                      << " -- " << e.destination 
                                      << " (вес: " << e.weight << ")" << std::endl;
                            std::cout << "    Объединили компоненты " << rootU 
                                      << " и " << rootV << std::endl;
                        }
                    }
                }
            }
            
            if (!addedAny)
            {
                // Если не добавили ни одного ребра, граф несвязный
                std::cout << "⚠ ВНИМАНИЕ: Не удалось добавить рёбра. Граф может быть несвязным." << std::endl;
                break;
            }
            
            if (verbose)
            {
                std::cout << "Текущий вес MST: " << totalWeight << std::endl;
                std::cout << "Количество рёбер в MST: " << mstEdges.size() << std::endl;
                uf.printState(numVertices);
            }
            
            iteration++;
            
            // Защита от бесконечного цикла (на случай ошибок)
            if (iteration > numVertices)
            {
                std::cout << "⚠ ВНИМАНИЕ: Превышено максимальное количество итераций." << std::endl;
                break;
            }
        }
        
        // Выводим результаты
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
        std::cout << "Минимальное остовное дерево (MST):" << std::endl;
        std::cout << "Рёбра MST:" << std::endl;
        
        for (size_t i = 0; i < mstEdges.size(); i++)
        {
            std::cout << "  " << mstEdges[i].source << " -- " 
                      << mstEdges[i].destination 
                      << " (вес: " << mstEdges[i].weight << ")" << std::endl;
        }
        
        std::cout << "\nОбщий вес MST: " << totalWeight << std::endl;
        std::cout << "Количество рёбер в MST: " << mstEdges.size() << std::endl;
        std::cout << "Количество вершин: " << numVertices << std::endl;
        std::cout << "Количество итераций: " << (iteration - 1) << std::endl;
        
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
    g1.boruvka();
    
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
    g2.boruvka();
    
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
    g3.boruvka();
    
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
    g4.boruvka();
    
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
    g5.boruvka();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Алгоритм Борувки:" << std::endl;
    std::cout << "- Находит минимальное остовное дерево (MST)" << std::endl;
    std::cout << "- Итеративный алгоритм: на каждой итерации обрабатывает все компоненты" << std::endl;
    std::cout << "- Использует Union-Find для управления компонентами связности" << std::endl;
    std::cout << "- Легко параллелизуется: поиск минимальных рёбер для компонентов независим" << std::endl;
    std::cout << "- Работает только с неориентированными связными графами" << std::endl;
    std::cout << "- Сложность: O(E log V) - максимум log V итераций" << std::endl;
    std::cout << "- Лучше для параллельных вычислений" << std::endl;
    std::cout << "- MST содержит V-1 рёбер для графа с V вершинами" << std::endl;
    std::cout << "- Применение: сети (электричество, вода), кластеризация, приближенные алгоритмы" << std::endl;
    
    return 0;
}


