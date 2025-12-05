#include <iostream>
#include <vector>
#include <iomanip>
#include <climits>

// Класс для представления взвешенного графа
class Graph
{
    int numVertices;                              // Количество вершин
    std::vector<std::vector<int>> dist;          // Матрица расстояний
    std::vector<std::vector<int>> next;          // Матрица для восстановления путей
    bool verbose;                                  // Режим детального вывода

public:
    // Конструктор: создаем граф с заданным количеством вершин
    Graph(int V, bool verboseMode = false)
    {
        numVertices = V;
        verbose = verboseMode;
        
        // Инициализируем матрицу расстояний
        dist.resize(V, std::vector<int>(V, INT_MAX / 2)); // Делим на 2 чтобы избежать переполнения
        next.resize(V, std::vector<int>(V, -1));
        
        // Расстояние от вершины до самой себя равно 0
        for (int i = 0; i < V; i++)
        {
            dist[i][i] = 0;
            next[i][i] = i;
        }
    }

    // Добавление взвешенного ребра от вершины u к вершине v
    void addEdge(int u, int v, int weight)
    {
        dist[u][v] = weight;
        next[u][v] = v;
    }

    // Вывод матрицы расстояний
    void printDistanceMatrix(const std::string& label)
    {
        std::cout << label << std::endl;
        std::cout << "    ";
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << std::setw(4) << i;
        }
        std::cout << std::endl;
        std::cout << "   ";
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << "----";
        }
        std::cout << std::endl;
        
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << i << " | ";
            for (int j = 0; j < numVertices; j++)
            {
                if (dist[i][j] == INT_MAX / 2)
                    std::cout << std::setw(6) << "∞";
                else
                    std::cout << std::setw(4) << dist[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Восстановление пути между двумя вершинами
    std::vector<int> reconstructPath(int u, int v)
    {
        std::vector<int> path;
        if (next[u][v] == -1)
            return path; // Пути нет
        
        path.push_back(u);
        while (u != v)
        {
            u = next[u][v];
            path.push_back(u);
        }
        return path;
    }

    // Вывод пути
    void printPath(int u, int v)
    {
        std::vector<int> path = reconstructPath(u, v);
        if (path.empty())
        {
            std::cout << "Путь не существует";
        }
        else
        {
            for (size_t i = 0; i < path.size(); i++)
            {
                std::cout << path[i];
                if (i < path.size() - 1)
                    std::cout << " -> ";
            }
        }
    }

    // Алгоритм Флойда-Уоршелла для поиска кратчайших путей между всеми парами вершин
    void floydWarshall()
    {
        std::cout << "\n=== Алгоритм Флойда-Уоршелла ===" << std::endl;
        std::cout << "Находит кратчайшие пути между ВСЕМИ парами вершин" << std::endl;
        std::cout << "Использует динамическое программирование" << std::endl;
        std::cout << "Сложность: O(V³) = O(" << numVertices << "³) = " 
                  << (numVertices * numVertices * numVertices) << " операций\n" << std::endl;
        
        if (verbose)
        {
            printDistanceMatrix("Начальная матрица расстояний:");
        }
        
        // Основной алгоритм: для каждой промежуточной вершины k
        for (int k = 0; k < numVertices; k++)
        {
            if (verbose)
            {
                std::cout << "========================================" << std::endl;
                std::cout << "Итерация " << (k + 1) << ": Используем вершину " << k 
                          << " как промежуточную" << std::endl;
                std::cout << "========================================" << std::endl;
            }
            
            int improvements = 0;
            
            // Для каждой пары вершин (i, j)
            for (int i = 0; i < numVertices; i++)
            {
                for (int j = 0; j < numVertices; j++)
                {
                    // Проверяем, можно ли улучшить путь от i до j через k
                    if (dist[i][k] != INT_MAX / 2 && dist[k][j] != INT_MAX / 2)
                    {
                        int newDist = dist[i][k] + dist[k][j];
                        
                        if (newDist < dist[i][j])
                        {
                            int oldDist = dist[i][j];
                            dist[i][j] = newDist;
                            next[i][j] = next[i][k];
                            improvements++;
                            
                            if (verbose && i != j)
                            {
                                std::cout << "  Улучшение: dist[" << i << "][" << j << "]: ";
                                if (oldDist == INT_MAX / 2)
                                    std::cout << "∞";
                                else
                                    std::cout << oldDist;
                                std::cout << " -> " << newDist;
                                std::cout << " (через " << k << ")" << std::endl;
                                std::cout << "    Путь: ";
                                printPath(i, j);
                                std::cout << ", расстояние = " << dist[i][j] << std::endl;
                            }
                        }
                    }
                }
            }
            
            if (verbose)
            {
                if (improvements == 0)
                {
                    std::cout << "  Улучшений не найдено" << std::endl;
                }
                else
                {
                    std::cout << "\nВсего улучшений: " << improvements << std::endl;
                }
                printDistanceMatrix("\nМатрица после итерации " + std::to_string(k + 1) + ":");
            }
        }
        
        // Проверка на наличие отрицательных циклов
        bool hasNegativeCycle = false;
        for (int i = 0; i < numVertices; i++)
        {
            if (dist[i][i] < 0)
            {
                hasNegativeCycle = true;
                std::cout << "\n⚠ ВНИМАНИЕ: Обнаружен отрицательный цикл в вершине " << i << std::endl;
            }
        }
        
        // Выводим результаты
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
        if (hasNegativeCycle)
        {
            std::cout << "⚠ Граф содержит отрицательный цикл!" << std::endl;
        }
        
        printDistanceMatrix("Финальная матрица кратчайших расстояний:");
        
        std::cout << "Примеры кратчайших путей:" << std::endl;
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (i != j && dist[i][j] != INT_MAX / 2)
                {
                    std::cout << "  " << i << " -> " << j << ": ";
                    printPath(i, j);
                    std::cout << " (длина: " << dist[i][j] << ")" << std::endl;
                }
            }
        }
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Простой направленный граф" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 1: Простой направленный граф
    //     0 --4--> 1
    //     |        |
    //     2        1
    //     |        |
    //     V        V
    //     2 --2--> 3
    Graph g1(4, false);
    g1.addEdge(0, 1, 4);
    g1.addEdge(0, 2, 2);
    g1.addEdge(1, 3, 1);
    g1.addEdge(2, 3, 2);
    g1.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Граф с несколькими путями" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 2: Граф с несколькими путями
    Graph g2(4, false);
    g2.addEdge(0, 1, 3);
    g2.addEdge(0, 3, 7);
    g2.addEdge(1, 0, 8);
    g2.addEdge(1, 2, 2);

    g2.addEdge(2, 0, 5);
    g2.addEdge(2, 3, 1);
    g2.addEdge(3, 0, 2);
    g2.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Полный граф (без детального вывода)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 3: Полный граф
    Graph g3(5, false);
    g3.addEdge(0, 1, 10);
    g3.addEdge(0, 2, 5);
    g3.addEdge(1, 2, 2);
    g3.addEdge(1, 3, 1);
    g3.addEdge(2, 1, 3);
    g3.addEdge(2, 3, 9);
    g3.addEdge(2, 4, 2);
    g3.addEdge(3, 4, 4);
    g3.addEdge(4, 3, 6);
    g3.addEdge(4, 0, 7);
    g3.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 4: Граф с отрицательными весами" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 4: Граф с отрицательными весами (но без отрицательных циклов)
    Graph g4(4, false);
    g4.addEdge(0, 1, 2);
    g4.addEdge(1, 2, -3);  // Отрицательный вес
    g4.addEdge(2, 3, 2);
    g4.addEdge(0, 3, 5);
    g4.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 5: Граф с ОТРИЦАТЕЛЬНЫМ ЦИКЛОМ" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 5: Граф с отрицательным циклом (обнаружение)
    // Цикл: 0 -> 1 -> 2 -> 0 с суммой весов: 1 + (-3) + (-2) = -4
    std::cout << "Граф содержит цикл: 0 -> 1 -> 2 -> 0" << std::endl;
    std::cout << "Сумма весов: 1 + (-3) + (-2) = -4 (отрицательный цикл!)" << std::endl;
    Graph g5(3, false);
    g5.addEdge(0, 1, 1);
    g5.addEdge(1, 2, -3);
    g5.addEdge(2, 0, -2);  // Замыкает отрицательный цикл
    g5.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 6: Несвязный граф" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 6: Несвязный граф (две отдельные компоненты)
    // Компонента 1: 0 <-> 1
    // Компонента 2: 2 <-> 3
    std::cout << "Граф состоит из двух несвязных компонент:" << std::endl;
    std::cout << "Компонента 1: 0 <-> 1" << std::endl;
    std::cout << "Компонента 2: 2 <-> 3" << std::endl;
    Graph g6(4, false);
    g6.addEdge(0, 1, 2);
    g6.addEdge(1, 0, 2);
    g6.addEdge(2, 3, 5);
    g6.addEdge(3, 2, 5);
    g6.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 7: Граф-звезда (один центр)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 7: Граф-звезда - все вершины соединены с центром
    //       1
    //       |3
    //   2---0---4
    //     5 |1 2
    //       3
    std::cout << "Все вершины соединены только с центральной вершиной 0" << std::endl;
    Graph g7(5, false);
    g7.addEdge(0, 1, 3);
    g7.addEdge(0, 2, 5);
    g7.addEdge(0, 3, 1);
    g7.addEdge(0, 4, 2);
    g7.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 8: Транзитивное замыкание (единичные веса)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 8: Граф с единичными весами - аналог BFS
    // Показывает минимальное количество шагов между вершинами
    std::cout << "Все рёбра имеют вес 1 (как в невзвешенном графе)" << std::endl;
    std::cout << "Результат показывает минимальное количество рёбер в пути" << std::endl;
    Graph g8(5, false);
    g8.addEdge(0, 1, 1);
    g8.addEdge(1, 2, 1);
    g8.addEdge(2, 3, 1);
    g8.addEdge(3, 4, 1);
    g8.addEdge(0, 2, 1);  // Прямой путь
    g8.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 9: Треугольное неравенство" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 9: Демонстрация улучшения путей через промежуточные вершины
    // Прямой путь 0->2 длиннее, чем 0->1->2
    std::cout << "Прямой путь 0->2 (вес 10) длиннее, чем 0->1->2 (вес 2+3=5)" << std::endl;
    std::cout << "Алгоритм найдёт более короткий путь через вершину 1" << std::endl;
    Graph g9(3, false);
    g9.addEdge(0, 1, 2);
    g9.addEdge(1, 2, 3);
    g9.addEdge(0, 2, 10);  // Неэффективный прямой путь
    g9.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 10: Симметричный граф (неориентированный)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 10: Симметричный граф (эмуляция неориентированного)
    //     0
    //    / \
    //   2   3
    //  1     2
    // /       \
    //1---4---2
    //     1
    std::cout << "Граф с симметричными рёбрами (неориентированный)" << std::endl;
    Graph g10(4, false);
    // Добавляем рёбра в обе стороны
    g10.addEdge(0, 1, 2);
    g10.addEdge(1, 0, 2);
    g10.addEdge(0, 2, 3);
    g10.addEdge(2, 0, 3);
    g10.addEdge(1, 3, 4);
    g10.addEdge(3, 1, 4);
    g10.addEdge(2, 3, 1);
    g10.addEdge(3, 2, 1);
    g10.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 11: Длинная цепь с обратным ребром" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 11: Длинная цепь, где обратное ребро создаёт короткий путь
    // 0->1->2->3->4 (веса 1,1,1,1) vs 0->4 (вес 10)
    std::cout << "Длинная цепь: 0->1->2->3->4 (сумма 4)" << std::endl;
    std::cout << "Прямое ребро: 0->4 (вес 10)" << std::endl;
    std::cout << "Обратный путь: 4->0->1 короче чем 4->3->2->1" << std::endl;
    Graph g11(5, false);
    g11.addEdge(0, 1, 1);
    g11.addEdge(1, 2, 1);
    g11.addEdge(2, 3, 1);

    g11.addEdge(3, 4, 1);
    g11.addEdge(0, 4, 10);  // Длинное прямое ребро
    g11.addEdge(4, 0, 2);   // Короткое обратное ребро
    g11.floydWarshall();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 12: Плотный граф (почти полный)" << std::endl;
    std::cout << "========================================" << std::endl;
    // Тест 12: Плотный граф с множеством рёбер
    std::cout << "Граф с большим количеством рёбер - показывает эффективность" << std::endl;
    std::cout << "для плотных графов (без детального вывода)" << std::endl;
    Graph g12(6, false);
    // Добавляем много рёбер с различными весами
    g12.addEdge(0, 1, 7);
    g12.addEdge(0, 2, 9);
    g12.addEdge(0, 5, 14);

    g12.addEdge(1, 2, 10);
    g12.addEdge(1, 3, 15);
    g12.addEdge(2, 3, 11);
    
    g12.addEdge(2, 5, 2);
    g12.addEdge(3, 4, 6);
    g12.addEdge(4, 5, 9);
    
    g12.addEdge(5, 1, 3);
    g12.addEdge(3, 0, 8);
    g12.addEdge(4, 1, 12);
    
    g12.floydWarshall();
    
    // std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    // std::cout << "Алгоритм Флойда-Уоршелла:" << std::endl;
    // std::cout << "- Находит кратчайшие пути между ВСЕМИ парами вершин" << std::endl;
    // std::cout << "- Использует динамическое программирование" << std::endl;
    // std::cout << "- Работает с отрицательными весами (но обнаруживает отрицательные циклы)" << std::endl;
    // std::cout << "- Сложность: O(V³) - медленнее Дейкстры для одной вершины" << std::endl;
    // std::cout << "- Но быстрее, чем запускать Дейкстру V раз для всех пар" << std::endl;
    // std::cout << "- Основная идея: пробуем использовать каждую вершину как промежуточную" << std::endl;
    
    return 0;
}

