#include <iostream>
#include <vector>
#include <queue>

// Вспомогательная функция для вывода содержимого очереди
void printQueue(std::queue<int> q, const std::string& label)
{
    std::cout << label << " [";
    bool first = true;
    while (!q.empty())
    {
        if (!first) std::cout << " ";
        std::cout << q.front();
        q.pop();
        first = false;
    }
    std::cout << "] (начало очереди слева, конец справа)" << std::endl;
}

// Класс для представления графа с использованием списка смежности
class Graph
{
    int numVertices;                    // Количество вершин
    std::vector<std::vector<int>> adj;  // Список смежности
    std::vector<bool> visited;          // Массив для отслеживания посещенных вершин
    bool verbose;                        // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        adj.resize(V);
        visited.resize(V, false);
        verbose = verboseMode;
    }

    // Добавление ребра между вершинами u и v (неориентированный граф)
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // Для неориентированного графа добавляем в обе стороны
    }

    // BFS (Breadth-First Search) - обход графа в ширину с детальным выводом
    void BFS(int startVertex)
    {
        // Сбрасываем массив посещенных вершин
        std::fill(visited.begin(), visited.end(), false);
        
        // Создаем очередь для хранения вершин
        std::queue<int> q;
        
        // Помечаем начальную вершину как посещенную и добавляем в очередь
        visited[startVertex] = true;
        q.push(startVertex);
        
        std::cout << "\n=== BFS с детальным выводом работы ОЧЕРЕДИ ===" << std::endl;
        std::cout << "Начальная вершина: " << startVertex << std::endl;
        if (verbose)
        {
            printQueue(q, "Очередь после добавления начальной вершины:");
        }
        
        std::cout << "\nПосещенные вершины: ";
        int step = 1;
        int level = 0;
        
        while (!q.empty())
        {
            if (verbose)
            {
                std::cout << "\n--- Шаг " << step << " (Уровень " << level << ") ---" << std::endl;
                printQueue(q, "Состояние очереди:");
            }
            
            // Извлекаем вершину из начала очереди (FIFO - первый пришел, первый ушел)
            int currentVertex = q.front();
            q.pop();
            
            if (verbose)
            {
                std::cout << "Извлекаем из начала очереди (pop): " << currentVertex << std::endl;
            }
            
            std::cout << currentVertex << " ";
            
            if (verbose)
            {
                std::cout << "\nИщем соседей вершины " << currentVertex << ": ";
                bool hasUnvisited = false;
                for (int neighbor : adj[currentVertex])
                {
                    std::cout << neighbor;
                    if (visited[neighbor])
                    {
                        std::cout << "(посещена) ";
                    }
                    else
                    {
                        std::cout << "(новая) ";
                        hasUnvisited = true;
                    }
                }
                std::cout << std::endl;
            }
            
            // Просматриваем всех соседей текущей вершины
            for (int neighbor : adj[currentVertex])
            {
                // Если сосед еще не посещен, помечаем его и добавляем в КОНЕЦ очереди
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                    if (verbose)
                    {
                        std::cout << "  -> Добавляем в конец очереди (push): " << neighbor << std::endl;
                    }
                }
            }
            
            if (verbose)
            {
                printQueue(q, "Очередь после обработки соседей:");
            }
            
            step++;
            // Определяем уровень: можно отслеживать через размер очереди на каждом шаге
        }
        std::cout << std::endl;
        std::cout << "\nОбход завершен!" << std::endl;
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Простое дерево" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 1: Простое дерево
    // Граф выглядит так:
    //     0
    //   /   \
    //  1     2
    //  | \   |
    //  3   4 5
    Graph g1(6, true);
    g1.addEdge(0, 1);
    g1.addEdge(0, 2);
    g1.addEdge(1, 3);
    g1.addEdge(1, 4);
    g1.addEdge(2, 5);
    g1.BFS(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Линейное дерево (цепочка)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 2: Линейное дерево (цепочка)
    // 0 -- 1 -- 2 -- 3 -- 4
    Graph g2(5, true);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 3);
    g2.addEdge(3, 4);
    g2.BFS(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Граф с циклом" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 3: Граф с циклом
    //     0
    //   /   \
    //  1 --- 2
    //  |     |
    //  3 --- 4
    Graph g3(5, true);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    g3.addEdge(1, 2);  // Цикл
    g3.addEdge(1, 3);
    g3.addEdge(2, 4);
    g3.addEdge(3, 4);  // Цикл
    g3.BFS(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 4: Сбалансированное дерево" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 4: Сбалансированное бинарное дерево
    //         0
    //       /   \
    //      1     2
    //     / \   / \
    //    3   4 5   6
    Graph g4(7, true);
    g4.addEdge(0, 1);
    g4.addEdge(0, 2);
    g4.addEdge(1, 3);
    g4.addEdge(1, 4);
    g4.addEdge(2, 5);
    g4.addEdge(2, 6);
    g4.BFS(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 5: Звездообразный граф" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 5: Звездообразный граф (одна центральная вершина)
    //       1
    //       |
    //   2 - 0 - 3
    //       |
    //       4
    Graph g5(5, true);
    g5.addEdge(0, 1);
    g5.addEdge(0, 2);
    g5.addEdge(0, 3);
    g5.addEdge(0, 4);
    g5.BFS(0);
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "BFS использует ОЧЕРЕДЬ (FIFO):" << std::endl;
    std::cout << "- Первый добавленный элемент извлекается первым" << std::endl;
    std::cout << "- Это приводит к обходу в ШИРИНУ" << std::endl;
    std::cout << "- Алгоритм посещает все вершины на одном уровне, затем переходит на следующий" << std::endl;
    std::cout << "- BFS находит кратчайший путь в невзвешенном графе" << std::endl;
    
    return 0;
}

