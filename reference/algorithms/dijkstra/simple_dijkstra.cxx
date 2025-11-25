#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>

// Вспомогательная функция для вывода содержимого приоритетной очереди
void printPriorityQueue(std::priority_queue<std::pair<int, int>, 
                                           std::vector<std::pair<int, int>>, 
                                           std::greater<std::pair<int, int>>> pq, 
                       const std::string& label)
{
    std::cout << label << " [";
    std::vector<std::pair<int, int>> temp;
    while (!pq.empty())
    {
        temp.push_back(pq.top());
        pq.pop();
    }
    // Выводим в обратном порядке, чтобы показать приоритетную очередь
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        std::cout << "(" << temp[i].first << "," << temp[i].second << ")";
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

// Класс для представления взвешенного графа
class Graph
{
    int numVertices;                              // Количество вершин
    std::vector<std::vector<Edge>> adj;          // Список смежности с весами
    std::vector<int> distance;                    // Массив кратчайших расстояний
    std::vector<bool> visited;                    // Массив для отслеживания обработанных вершин
    bool verbose;                                  // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        adj.resize(V);
        distance.resize(V, INT_MAX); // Инициализируем все расстояния как бесконечность
        visited.resize(V, false);
        verbose = verboseMode;
    }

    // Добавление взвешенного ребра от вершины u к вершине v (ориентированный граф)
    void addEdge(int u, int v, int weight)
    {
        Edge e;
        e.destination = v;
        e.weight = weight;
        adj[u].push_back(e);
    }

    // Вывод текущих расстояний
    void printDistances(int step)
    {
        std::cout << "Расстояния: [";
        for (int i = 0; i < numVertices; i++)
        {
            if (distance[i] == INT_MAX)
                std::cout << "∞";
            else
                std::cout << distance[i];
            if (i < numVertices - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    // Алгоритм Дейкстры для поиска кратчайших путей от начальной вершины
    void dijkstra(int startVertex)
    {
        // Инициализация: расстояние до начальной вершины равно 0
        distance[startVertex] = 0;
        
        // Используем приоритетную очередь (минимум наверху)
        // Храним пары: (расстояние, вершина)
        std::priority_queue<std::pair<int, int>, 
                           std::vector<std::pair<int, int>>, 
                           std::greater<std::pair<int, int>>> pq;
        
        // Добавляем начальную вершину в очередь
        pq.push({0, startVertex});
        
        std::cout << "\n=== Алгоритм Дейкстры с детальным выводом работы ПРИОРИТЕТНОЙ ОЧЕРЕДИ ===" << std::endl;
        std::cout << "Начальная вершина: " << startVertex << std::endl;
        if (verbose)
        {
            printDistances(0);
            printPriorityQueue(pq, "Приоритетная очередь после инициализации:");
        }
        
        int step = 1;
        
        while (!pq.empty())
        {
            if (verbose)
            {
                std::cout << "\n--- Шаг " << step << " ---" << std::endl;
                printPriorityQueue(pq, "Состояние приоритетной очереди:");
            }
            
            // Извлекаем вершину с минимальным расстоянием
            int u = pq.top().second;
            int dist_u = pq.top().first;
            pq.pop();
            
            // Если вершина уже обработана, пропускаем
            if (visited[u])
            {
                if (verbose)
                {
                    std::cout << "Вершина " << u << " уже обработана, пропускаем" << std::endl;
                }
                continue;
            }
            
            // Помечаем вершину как обработанную
            visited[u] = true;
            
            if (verbose)
            {
                std::cout << "Обрабатываем вершину " << u << " (расстояние: " << dist_u << ")" << std::endl;
            }
            
            // Просматриваем всех соседей текущей вершины
            for (const Edge& edge : adj[u])
            {
                int v = edge.destination;
                int weight = edge.weight;
                
                if (verbose)
                {
                    std::cout << "  Проверяем соседа " << v << " (вес ребра: " << weight << ")" << std::endl;
                    std::cout << "    Текущее расстояние до " << v << ": ";
                    if (distance[v] == INT_MAX)
                        std::cout << "∞";
                    else
                        std::cout << distance[v];
                    std::cout << std::endl;
                    std::cout << "    Новое расстояние через " << u << ": " << distance[u] << " + " << weight << " = " << (distance[u] + weight) << std::endl;
                }
                
                // Если найден более короткий путь к вершине v
                if (!visited[v] && distance[u] != INT_MAX && 
                    distance[u] + weight < distance[v])
                {
                    int oldDist = distance[v];
                    // Обновляем расстояние до вершины v
                    distance[v] = distance[u] + weight;
                    // Добавляем вершину v в очередь с новым расстоянием
                    pq.push({distance[v], v});
                    
                    if (verbose)
                    {
                        std::cout << "    ✓ Обновляем расстояние: " << oldDist << " -> " << distance[v] << std::endl;
                        std::cout << "    ✓ Добавляем в приоритетную очередь: (" << distance[v] << ", " << v << ")" << std::endl;
                    }
                }
                else if (verbose)
                {
                    std::cout << "    ✗ Расстояние не улучшается, пропускаем" << std::endl;
                }
            }
            
            if (verbose)
            {
                printDistances(step);
            }
            
            step++;
        }
        
        // Выводим результаты
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
        std::cout << "Кратчайшие расстояния от вершины " << startVertex << ":" << std::endl;
        for (int i = 0; i < numVertices; i++)
        {
            if (distance[i] == INT_MAX)
            {
                std::cout << "  Вершина " << i << ": недостижима" << std::endl;
            }
            else
            {
                std::cout << "  Вершина " << i << ": " << distance[i] << std::endl;
            }
        }
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Простой взвешенный граф" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 1: Простой взвешенный граф
    //     0 --3--> 1 --1--> 3
    //     |        |
    //     2        4
    //     |        |
    //     5        2
    //     |
    //     1
    Graph g1(6, true);
    g1.addEdge(0, 1, 3);
    g1.addEdge(0, 2, 2);
    g1.addEdge(1, 3, 1);
    g1.addEdge(1, 4, 4);
    g1.addEdge(2, 5, 1);
    g1.addEdge(4, 2, 2);
    g1.dijkstra(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Граф с несколькими путями" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 2: Граф с несколькими путями к одной вершине
    //     0 --1--> 1 --3--> 3
    //     |        |        |
    //     4        2        1
    //     |        |        |
    //     1        1        4
    //     |        |
    //     3 --2--> 2
    Graph g2(5, true);
    g2.addEdge(0, 1, 1);
    g2.addEdge(0, 3, 4);
    g2.addEdge(1, 2, 2);
    g2.addEdge(1, 3, 3);
    g2.addEdge(2, 3, 1);
    g2.addEdge(3, 4, 1);
    g2.dijkstra(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Дерево с весами" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 3: Дерево с весами
    //         0
    //       /   \
    //      5     3
    //     / \   / \
    //    1   2 4   1
    //    1   2 4   1
    Graph g3(7, true);
    g3.addEdge(0, 1, 5);
    g3.addEdge(0, 2, 3);
    g3.addEdge(1, 3, 1);
    g3.addEdge(1, 4, 2);
    g3.addEdge(2, 5, 4);
    g3.addEdge(2, 6, 1);
    g3.dijkstra(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 4: Граф с циклом (без детального вывода)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 4: Граф с циклом
    Graph g4(4, false);
    g4.addEdge(0, 1, 1);
    g4.addEdge(1, 2, 2);
    g4.addEdge(2, 3, 3);
    g4.addEdge(3, 0, 4);  // Цикл
    g4.addEdge(1, 3, 5);
    g4.dijkstra(0);
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Алгоритм Дейкстры использует ПРИОРИТЕТНУЮ ОЧЕРЕДЬ:" << std::endl;
    std::cout << "- Всегда обрабатывает вершину с минимальным расстоянием" << std::endl;
    std::cout << "- Это гарантирует нахождение кратчайших путей" << std::endl;
    std::cout << "- Работает только с неотрицательными весами ребер" << std::endl;
    std::cout << "- Похож на BFS, но учитывает веса ребер" << std::endl;
    
    return 0;
}

