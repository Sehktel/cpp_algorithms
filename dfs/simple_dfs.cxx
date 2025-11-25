#include <iostream>
#include <vector>
#include <stack>

// Вспомогательная функция для вывода содержимого стека
void printStack(std::stack<int> s, const std::string& label)
{
    std::cout << label << " [";
    std::vector<int> temp;
    while (!s.empty())
    {
        temp.push_back(s.top());
        s.pop();
    }
    // Выводим в обратном порядке, чтобы показать стек сверху вниз
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        std::cout << temp[i];
        if (i > 0) std::cout << " ";
    }
    std::cout << "] (верх стека справа)" << std::endl;
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

    // DFS с использованием стека (итеративный подход) с детальным выводом
    void DFS(int startVertex)
    {
        // Сбрасываем массив посещенных вершин
        std::fill(visited.begin(), visited.end(), false);
        
        // Создаем стек для хранения вершин
        std::stack<int> stack;
        
        // Помечаем начальную вершину как посещенную и добавляем в стек
        visited[startVertex] = true;
        stack.push(startVertex);
        
        std::cout << "\n=== DFS с детальным выводом работы СТЕКА ===" << std::endl;
        std::cout << "Начальная вершина: " << startVertex << std::endl;
        if (verbose)
        {
            printStack(stack, "Стек после добавления начальной вершины:");
        }
        
        std::cout << "\nПосещенные вершины: ";
        int step = 1;
        
        while (!stack.empty())
        {
            if (verbose)
            {
                std::cout << "\n--- Шаг " << step << " ---" << std::endl;
                printStack(stack, "Состояние стека:");
            }
            
            // Извлекаем вершину из стека (LIFO - последний пришел, первый ушел)
            int currentVertex = stack.top();
            stack.pop();
            
            if (verbose)
            {
                std::cout << "Извлекаем из стека (pop): " << currentVertex << std::endl;
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
            // ВАЖНО: добавляем в обратном порядке, чтобы обход был предсказуемым
            std::vector<int> neighbors;
            for (int neighbor : adj[currentVertex])
            {
                neighbors.push_back(neighbor);
            }
            
            // Добавляем соседей в обратном порядке (чтобы первый сосед был наверху стека)
            for (int i = neighbors.size() - 1; i >= 0; i--)
            {
                int neighbor = neighbors[i];
                // Если сосед еще не посещен, помечаем его и добавляем в стек
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    stack.push(neighbor);
                    if (verbose)
                    {
                        std::cout << "  -> Добавляем в стек (push): " << neighbor << std::endl;
                    }
                }
            }
            
            if (verbose)
            {
                printStack(stack, "Стек после обработки соседей:");
            }
            
            step++;
        }
        std::cout << std::endl;
        std::cout << "\nОбход завершен!" << std::endl;
    }

    // DFS с использованием рекурсии (рекурсивный подход)
    void DFSRecursive(int vertex)
    {
        // Помечаем текущую вершину как посещенную
        visited[vertex] = true;
        std::cout << vertex << " ";
        
        // Рекурсивно посещаем всех непосещенных соседей
        for (int neighbor : adj[vertex])
        {
            if (!visited[neighbor])
            {
                DFSRecursive(neighbor);
            }
        }
    }

    // Обертка для рекурсивного DFS
    void DFSRecursiveWrapper(int startVertex)
    {
        std::fill(visited.begin(), visited.end(), false);
        std::cout << "DFS (recursive) traversal starting from vertex " << startVertex << ": ";
        DFSRecursive(startVertex);
        std::cout << std::endl;
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
    g1.DFS(0);
    
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
    g2.DFS(0);
    
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
    g3.DFS(0);
    
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
    g4.DFS(0);
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 5: Рекурсивный DFS (для сравнения)" << std::endl;
    std::cout << "========================================" << std::endl;
    Graph g5(6, false);
    g5.addEdge(0, 1);
    g5.addEdge(0, 2);
    g5.addEdge(1, 3);
    g5.addEdge(1, 4);
    g5.addEdge(2, 5);
    g5.DFSRecursiveWrapper(0);
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "DFS использует СТЕК (LIFO):" << std::endl;
    std::cout << "- Последний добавленный элемент извлекается первым" << std::endl;
    std::cout << "- Это приводит к обходу в ГЛУБИНУ" << std::endl;
    std::cout << "- Алгоритм идет как можно дальше, затем возвращается назад" << std::endl;
    
    return 0;
}

