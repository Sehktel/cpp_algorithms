#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>

// Вспомогательная функция для вывода содержимого приоритетной очереди
void printPriorityQueue(std::priority_queue<std::pair<int, std::pair<int, int>>, 
                                           std::vector<std::pair<int, std::pair<int, int>>>, 
                                           std::greater<std::pair<int, std::pair<int, int>>>> pq, 
                       const std::string& label)
{
    std::cout << label << " [";
    std::vector<std::pair<int, std::pair<int, int>>> temp;
    while (!pq.empty())
    {
        temp.push_back(pq.top());
        pq.pop();
    }
    // Выводим в обратном порядке, чтобы показать приоритетную очередь
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        std::cout << "(" << temp[i].first << ": " << temp[i].second.first 
                  << "->" << temp[i].second.second << ")";
        if (i > 0) std::cout << " ";
    }
    std::cout << "] (минимум наверху)" << std::endl;
}

// Структура для представления ребра графа
struct Edge
{
    int destination; // Конечная вершина
    int weight;      // Вес ребра
};

// Класс для представления неориентированного взвешенного графа
class Graph
{
    int numVertices;                              // Количество вершин
    std::vector<std::vector<Edge>> adj;          // Список смежности с весами
    std::vector<bool> inMST;                      // Вершины уже включенные в MST
    std::vector<int> parent;                      // Родитель каждой вершины в MST
    std::vector<int> key;                         // Минимальный вес ребра для включения в MST
    bool verbose;                                  // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        adj.resize(V);
        inMST.resize(V, false);
        parent.resize(V, -1);
        key.resize(V, INT_MAX);
        verbose = verboseMode;
    }

    // Добавление взвешенного ребра между вершинами u и v (неориентированный граф)
    void addEdge(int u, int v, int weight)
    {
        Edge e1, e2;
        e1.destination = v;
        e1.weight = weight;
        adj[u].push_back(e1);
        
        e2.destination = u;
        e2.weight = weight;
        adj[v].push_back(e2);
    }

    // Вывод текущих ключей
    void printKeys()
    {
        std::cout << "Ключи (мин. веса): [";
        for (int i = 0; i < numVertices; i++)
        {
            if (key[i] == INT_MAX)
                std::cout << "∞";
            else
                std::cout << key[i];
            if (i < numVertices - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    // Вывод текущего состояния MST
    void printMSTState()
    {
        std::cout << "Вершины в MST: [";
        bool first = true;
        for (int i = 0; i < numVertices; i++)
        {
            if (inMST[i])
            {
                if (!first) std::cout << ", ";
                std::cout << i;
                first = false;
            }
        }
        std::cout << "]" << std::endl;
    }

    // Алгоритм Прима для построения минимального остовного дерева (MST)
    void prim(int startVertex = 0)
    {
        // Используем приоритетную очередь (минимум наверху)
        // Храним тройки: (вес, (откуда, куда))
        std::priority_queue<std::pair<int, std::pair<int, int>>, 
                           std::vector<std::pair<int, std::pair<int, int>>>, 
                           std::greater<std::pair<int, std::pair<int, int>>>> pq;
        
        std::cout << "\n=== Алгоритм Прима для построения Минимального Остовного Дерева (MST) ===" << std::endl;
        std::cout << "Начальная вершина: " << startVertex << std::endl;
        std::cout << "MST - это подграф, который:" << std::endl;
        std::cout << "  1. Включает все вершины" << std::endl;
        std::cout << "  2. Является деревом (связный, без циклов)" << std::endl;
        std::cout << "  3. Имеет минимальную сумму весов рёбер\n" << std::endl;
        
        // Начинаем с произвольной вершины
        key[startVertex] = 0;
        pq.push({0, {-1, startVertex}}); // (вес, (родитель, вершина))
        
        std::vector<std::pair<int, int>> mstEdges; // Рёбра MST для финального вывода
        int totalWeight = 0;
        int step = 1;
        
        if (verbose)
        {
            printKeys();
            printMSTState();
        }
        
        while (!pq.empty())
        {
            // Извлекаем ребро с минимальным весом
            int weight = pq.top().first;
            int from = pq.top().second.first;
            int to = pq.top().second.second;
            pq.pop();
            
            // Если вершина уже в MST, пропускаем
            if (inMST[to])
            {
                if (verbose)
                {
                    std::cout << "Вершина " << to << " уже в MST, пропускаем" << std::endl;
                }
                continue;
            }
            
            if (verbose)
            {
                std::cout << "\n--- Шаг " << step << " ---" << std::endl;
                std::cout << "Выбираем ребро: ";
                if (from == -1)
                    std::cout << "START -> " << to;
                else
                    std::cout << from << " -> " << to;
                std::cout << " (вес: " << weight << ")" << std::endl;
            }
            
            // Добавляем вершину в MST
            inMST[to] = true;
            parent[to] = from;
            
            if (from != -1)
            {
                mstEdges.push_back({from, to});
                totalWeight += weight;
            }
            
            if (verbose)
            {
                std::cout << "Добавили вершину " << to << " в MST" << std::endl;
                printMSTState();
                std::cout << "Текущий вес MST: " << totalWeight << std::endl;
            }
            
            // Просматриваем всех соседей добавленной вершины
            if (verbose && !adj[to].empty())
            {
                std::cout << "\nИщем соседей вершины " << to << ":" << std::endl;
            }
            
            for (const Edge& edge : adj[to])
            {
                int v = edge.destination;
                int w = edge.weight;
                
                // Если сосед не в MST и вес ребра меньше текущего ключа
                if (!inMST[v])
                {
                    if (verbose)
                    {
                        std::cout << "  Сосед " << v << " (вес ребра: " << w << ")";
                        if (w < key[v])
                        {
                            std::cout << " - УЛУЧШАЕМ ключ: ";
                            if (key[v] == INT_MAX)
                                std::cout << "∞";
                            else
                                std::cout << key[v];
                            std::cout << " -> " << w;
                        }
                        else
                        {
                            std::cout << " - не улучшает (текущий ключ: ";
                            if (key[v] == INT_MAX)
                                std::cout << "∞";
                            else
                                std::cout << key[v];
                            std::cout << ")";
                        }
                        std::cout << std::endl;
                    }
                    
                    if (w < key[v])
                    {
                        key[v] = w;
                        pq.push({w, {to, v}});
                    }
                }
                else if (verbose)
                {
                    std::cout << "  Сосед " << v << " уже в MST, пропускаем" << std::endl;
                }
            }
            
            if (verbose)
            {
                printKeys();
                printPriorityQueue(pq, "Приоритетная очередь:");
            }
            
            step++;
        }
        
        // Выводим результаты
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
        std::cout << "Минимальное остовное дерево (MST):" << std::endl;
        std::cout << "Рёбра MST:" << std::endl;
        
        // Визуализация дерева
        for (size_t i = 0; i < mstEdges.size(); i++)
        {
            int u = mstEdges[i].first;
            int v = mstEdges[i].second;
            
            // Находим вес ребра
            int edgeWeight = 0;
            for (const Edge& e : adj[u])
            {
                if (e.destination == v)
                {
                    edgeWeight = e.weight;
                    break;
                }
            }
            
            std::cout << "  " << u << " -- " << v << " (вес: " << edgeWeight << ")" << std::endl;
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
    //     |
    //     1
    //     3
    Graph g1(4, true);
    g1.addEdge(0, 1, 1);
    g1.addEdge(0, 2, 4);
    g1.addEdge(1, 2, 2);
    g1.addEdge(2, 3, 1);
    g1.prim(0);
    
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
    g2.prim(0);
    
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
    g3.prim(0);
    
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
    g4.prim(0);
    
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
    g5.prim(0);
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Алгоритм Прима:" << std::endl;
    std::cout << "- Находит минимальное остовное дерево (MST)" << std::endl;
    std::cout << "- Жадный алгоритм: всегда выбирает ребро с минимальным весом" << std::endl;
    std::cout << "- Похож на алгоритм Дейкстры (использует приоритетную очередь)" << std::endl;
    std::cout << "- Работает только с неориентированными связными графами" << std::endl;
    std::cout << "- Сложность: O(E log V) с приоритетной очередью" << std::endl;
    std::cout << "- MST содержит V-1 рёбер для графа с V вершинами" << std::endl;
    std::cout << "- Применение: сети (электричество, вода), кластеризация, приближенные алгоритмы" << std::endl;
    
    return 0;
}


