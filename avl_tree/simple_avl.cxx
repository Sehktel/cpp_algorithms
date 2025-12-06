#include <iostream>
#include <algorithm>

// Узел AVL дерева
struct AVLNode
{
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;  // Высота поддерева с корнем в этом узле
    
    AVLNode(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

// Класс AVL дерева (самобалансирующееся бинарное дерево поиска)
class AVLTree
{
    AVLNode* root;
    bool verbose;

    // Получить высоту узла
    int getHeight(AVLNode* node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    // Получить баланс-фактор узла (разница высот левого и правого поддеревьев)
    int getBalance(AVLNode* node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // Обновить высоту узла
    void updateHeight(AVLNode* node)
    {
        if (node != nullptr)
        {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    // ========================================================================
    // БАЗОВЫЕ ПОВОРОТЫ: Почему только два?
    // ========================================================================
    // В AVL дереве достаточно реализовать только ДВА базовых поворота:
    // 1. Правый поворот (rightRotate) - для левого перевеса
    // 2. Левый поворот (leftRotate) - для правого перевеса
    //
    // Все остальные случаи (LL, RR, LR, RL) собираются из этих двух примитивов.
    // Это аналогично тому, как в геометрии любой поворот можно разложить на
    // комбинацию поворотов вокруг осей X и Y.
    //
    // Примеры комбинаций:
    // - LL (Left-Left):   только rightRotate
    // - RR (Right-Right): только leftRotate
    // - LR (Left-Right):  leftRotate(левое поддерево) + rightRotate(корень)
    // - RL (Right-Left):  rightRotate(правое поддерево) + leftRotate(корень)
    // ========================================================================

    // Правый поворот (Right Rotate) - базовый примитив #1
    // Используется для исправления левого перевеса
    //
    // Визуализация поворота:
    //        y                  x
    //       / \                / \
    //      x   T3    =>      T1   y
    //     / \                    / \
    //    T1  T2                 T2  T3
    //
    // Параметры:
    //   y - узел с дисбалансом (левое поддерево выше правого)
    // Возвращает:
    //   x - новый корень поддерева после поворота
    AVLNode* rightRotate(AVLNode* y)
    {
        if (verbose)
            std::cout << "  [БАЗОВЫЙ] Правый поворот вокруг " << y->data << std::endl;
        
        AVLNode* x = y->left;      // Левый потомок становится новым корнем
        AVLNode* T2 = x->right;    // Сохраняем правое поддерево x

        // Выполняем поворот: x становится корнем, y становится правым потомком x
        x->right = y;
        y->left = T2;  // T2 "переезжает" к y

        // Обновляем высоты (сначала y, потом x, т.к. x зависит от y)
        updateHeight(y);
        updateHeight(x);

        return x; // Новый корень поддерева
    }

    // Левый поворот (Left Rotate) - базовый примитив #2
    // Используется для исправления правого перевеса
    //
    // Визуализация поворота:
    //      x                      y
    //     / \                    / \
    //    T1  y        =>        x   T3
    //       / \                / \
    //      T2  T3             T1  T2
    //
    // Параметры:
    //   x - узел с дисбалансом (правое поддерево выше левого)
    // Возвращает:
    //   y - новый корень поддерева после поворота
    AVLNode* leftRotate(AVLNode* x)
    {
        if (verbose)
            std::cout << "  [БАЗОВЫЙ] Левый поворот вокруг " << x->data << std::endl;
        
        AVLNode* y = x->right;     // Правый потомок становится новым корнем
        AVLNode* T2 = y->left;     // Сохраняем левое поддерево y

        // Выполняем поворот: y становится корнем, x становится левым потомком y
        y->left = x;
        x->right = T2;  // T2 "переезжает" к x

        // Обновляем высоты (сначала x, потом y, т.к. y зависит от x)
        updateHeight(x);
        updateHeight(y);

        return y; // Новый корень поддерева
    }

    // ========================================================================
    // КАК ВРУЧНУЮ РАСПОЗНАТЬ ТИП ПОВОРОТА?
    // ========================================================================
    // Алгоритм определения типа дисбаланса (пошагово):
    //
    // ШАГ 1: Найдите узел с дисбалансом
    //   - balance > 1  => левое поддерево выше (перевес слева)
    //   - balance < -1 => правое поддерево выше (перевес справа)
    //
    // ШАГ 2: Определите, в каком поддереве находится новый элемент
    //   - Если balance > 1: новый элемент в ЛЕВОМ поддереве
    //   - Если balance < -1: новый элемент в ПРАВОМ поддереве
    //
    // ШАГ 3: Определите, в каком поддереве потомка находится новый элемент
    //   Для balance > 1 (перевес слева):
    //     - val < node->left->data => новый элемент в ЛЕВОМ поддереве left
    //     - val > node->left->data => новый элемент в ПРАВОМ поддереве left
    //
    //   Для balance < -1 (перевес справа):
    //     - val > node->right->data => новый элемент в ПРАВОМ поддереве right
    //     - val < node->right->data => новый элемент в ЛЕВОМ поддереве right
    //
    // ШАГ 4: Определите тип поворота по комбинации
    //   Первая буква = где перевес (L или R)
    //   Вторая буква = где новый элемент относительно потомка (L или R)
    //
    //   Примеры:
    //   - balance > 1 && val < node->left->data => LL (Left-Left)
    //     Перевес слева, новый элемент слева от left
    //
    //   - balance > 1 && val > node->left->data => LR (Left-Right)
    //     Перевес слева, новый элемент справа от left
    //
    //   - balance < -1 && val > node->right->data => RR (Right-Right)
    //     Перевес справа, новый элемент справа от right
    //
    //   - balance < -1 && val < node->right->data => RL (Right-Left)
    //     Перевес справа, новый элемент слева от right
    //
    // ВИЗУАЛЬНЫЕ ПРИМЕРЫ:
    //
    // СЛУЧАЙ LL (Left-Left):
    //        50 (balance=2)         30
    //       /                      /  \
    //     30 (balance=1)    =>   20   50
    //    /                      /
    //  20 (новый)             10
    //  /
    // 10
    // Признаки: перевес слева, новый элемент слева от левого потомка
    //
    // СЛУЧАЙ RR (Right-Right):
    //   10 (balance=-2)             30
    //    \                         /  \
    //     30 (balance=-1)   =>   10   50
    //      \                           \
    //       50 (новый)                  70
    //        \
    //         70
    // Признаки: перевес справа, новый элемент справа от правого потомка
    //
    // СЛУЧАЙ LR (Left-Right):
    //        50 (balance=2)         50         40
    //       /                       /          /  \
    //     30 (balance=-1)    =>   40    =>   30   50
    //      \                     /
    //       40 (новый)         30
    // Признаки: перевес слева, новый элемент справа от левого потомка
    // Решение: сначала левый поворот вокруг 30, потом правый вокруг 50
    //
    // СЛУЧАЙ RL (Right-Left):
    //   10 (balance=-2)        10           20
    //    \                      \           / \
    //     30 (balance=1)   =>    20    =>  10  30
    //    /                        \
    //   20 (новый)                30
    // Признаки: перевес справа, новый элемент слева от правого потомка
    // Решение: сначала правый поворот вокруг 30, потом левый вокруг 10
    //
    // ПРАКТИЧЕСКИЙ АЛГОРИТМ (для удаления):
    // При удалении мы не знаем, где был удаленный элемент, поэтому
    // проверяем баланс потомков напрямую:
    //   - balance > 1 && getBalance(node->left) >= 0 => LL
    //   - balance > 1 && getBalance(node->left) < 0  => LR
    //   - balance < -1 && getBalance(node->right) <= 0 => RR
    //   - balance < -1 && getBalance(node->right) > 0  => RL
    // ========================================================================

    // ========================================================================
    // ВСТАВКА С БАЛАНСИРОВКОЙ
    // ========================================================================
    // НАПРАВЛЕНИЕ ПРОВЕРКИ БАЛАНСА: СНИЗУ ВВЕРХ (bottom-up)
    //
    // Процесс работает следующим образом:
    // 1. Рекурсивно спускаемся вниз по дереву до места вставки
    // 2. Вставляем новый узел
    // 3. При возврате из рекурсии (снизу вверх) проверяем баланс каждого узла
    // 4. Если находим дисбаланс - выполняем поворот
    //
    // Почему снизу вверх?
    // - Вставка нового узла может нарушить баланс только в узлах на пути
    //   от корня до места вставки
    // - Проверка при возврате из рекурсии гарантирует, что мы проверяем
    //   все потенциально затронутые узлы
    // - Это эффективнее, чем проверять все дерево сверху вниз
    //
    // МНОЖЕСТВЕННЫЕ БАЛАНСИРОВКИ:
    // Теоретически после одного поворота баланс может нарушиться в родительском узле.
    // Однако на практике:
    // - Один поворот восстанавливает баланс в текущем узле
    // - Высота поддерева после поворота НЕ увеличивается (может уменьшиться)
    // - Поэтому родительские узлы обычно не требуют дополнительной балансировки
    // - В худшем случае может потребоваться O(log n) поворотов, но это крайне редко
    //
    // БАЛАНСИРОВКА ПРИ ВСТАВКЕ:
    // Дерево ВСЕГДА балансируется по мере добавления вершин.
    // Каждая вставка проверяет и восстанавливает баланс, поэтому дерево
    // остается сбалансированным после каждой операции.
    //
    // ЧТО ЕСЛИ ДЕРЕВО УЖЕ ЗАДАНО (несбалансированное)?
    // Если у вас есть уже построенное несбалансированное дерево, есть два подхода:
    //
    // ПОДХОД 1: Построить заново (рекомендуется)
    //   - Собрать все элементы через inorder обход: O(n)
    //   - Построить новое AVL дерево, вставляя элементы по одному: O(n log n)
    //   - Итого: O(n log n), но гарантированно сбалансированное дерево
    //
    // ПОДХОД 2: Балансировать существующее дерево
    //   - Можно использовать алгоритм DSW (Day-Stout-Warren) для преобразования
    //     любого BST в полностью сбалансированное дерево: O(n)
    //   - Затем применить повороты для приведения к AVL условиям
    //   - Более сложно в реализации, но быстрее
    //
    // В данной реализации используется ПОДХОД 1: все элементы вставляются
    // по одному через insert(), который автоматически поддерживает баланс.
    // ========================================================================
    AVLNode* insertHelper(AVLNode* node, int val)
    {
        // ========== ЭТАП 1: Рекурсивный спуск и вставка ==========
        // Обычная вставка в BST (бинарное дерево поиска)
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Создаем новый узел: " << val << std::endl;
            return new AVLNode(val);
        }

        // Рекурсивно идем вниз по дереву
        if (val < node->data)
        {
            node->left = insertHelper(node->left, val);
        }
        else if (val > node->data)
        {
            node->right = insertHelper(node->right, val);
        }
        else
        {
            // Дубликаты не допускаются
            return node;
        }

        // ========== ЭТАП 2: Проверка баланса при возврате (СНИЗУ ВВЕРХ) ==========
        // Мы здесь только после возврата из рекурсии, т.е. идем снизу вверх
        
        // Обновляем высоту текущего узла (она могла измениться после вставки)
        updateHeight(node);

        // Получаем баланс-фактор: разница высот левого и правого поддеревьев
        // balance > 1  => левое поддерево выше (нужен правый поворот)
        // balance < -1 => правое поддерево выше (нужен левый поворот)
        // -1 <= balance <= 1 => узел сбалансирован
        int balance = getBalance(node);

        if (verbose && balance != 0)
        {
            std::cout << "  [Проверка баланса снизу вверх] Узел " << node->data 
                      << ": баланс = " << balance << std::endl;
        }

        // ========== ЭТАП 3: Восстановление баланса (если нужно) ==========
        // Здесь применяем алгоритм распознавания типа поворота:
        // 1. balance > 1 => перевес слева (L)
        // 2. balance < -1 => перевес справа (R)
        // 3. Сравниваем val с node->left->data или node->right->data
        //    чтобы определить вторую букву (L или R)

        // СЛУЧАЙ 1: Left-Left (LL) - левое поддерево левого потомка перевешивает
        // Распознавание: balance > 1 (перевес слева) И val < node->left->data (новый элемент слева от left)
        // Структура:     node          После поворота:    left
        //                / \                              /   \
        //             left  ...                        ...   node
        //             /                                          \
        //          new_val                                      ...
        // Решение: ОДИН правый поворот вокруг node
        if (balance > 1 && val < node->left->data)
        {
            if (verbose)
                std::cout << "  [LL] Left-Left: один правый поворот" << std::endl;
            return rightRotate(node);  // Используем базовый примитив #1
        }

        // СЛУЧАЙ 2: Right-Right (RR) - правое поддерево правого потомка перевешивает
        // Распознавание: balance < -1 (перевес справа) И val > node->right->data (новый элемент справа от right)
        // Структура:   node          После поворота:      right
        //             /   \                              /     \
        //          ...   right                        node     ...
        //                  \                          /
        //                 new_val                   ...
        // Решение: ОДИН левый поворот вокруг node
        if (balance < -1 && val > node->right->data)
        {
            if (verbose)
                std::cout << "  [RR] Right-Right: один левый поворот" << std::endl;
            return leftRotate(node);  // Используем базовый примитив #2
        }

        // СЛУЧАЙ 3: Left-Right (LR) - правое поддерево левого потомка перевешивает
        // Распознавание: balance > 1 (перевес слева) И val > node->left->data (новый элемент справа от left)
        // Структура:      node                    node
        //                 / \                     / \
        //              left  ...    =>         new  ...
        //              /  \                     /
        //           ...   new                left
        // Решение: КОМБИНАЦИЯ двух поворотов
        // 1. Левый поворот вокруг left (превращает LR в LL)
        // 2. Правый поворот вокруг node (исправляет LL)
        if (balance > 1 && val > node->left->data)
        {
            if (verbose)
                std::cout << "  [LR] Left-Right: комбинация (левый + правый)" << std::endl;
            // Сначала левый поворот левого поддерева (базовый примитив #2)
            node->left = leftRotate(node->left);
            // Затем правый поворот корня (базовый примитив #1)
            return rightRotate(node);
        }

        // СЛУЧАЙ 4: Right-Left (RL) - левое поддерево правого потомка перевешивает
        // Распознавание: balance < -1 (перевес справа) И val < node->right->data (новый элемент слева от right)
        // Структура:   node                  node
        //             /   \                 /   \
        //          ...   right    =>      ...   new
        //                 /                        \
        //              new                        right
        // Решение: КОМБИНАЦИЯ двух поворотов
        // 1. Правый поворот вокруг right (превращает RL в RR)
        // 2. Левый поворот вокруг node (исправляет RR)
        if (balance < -1 && val < node->right->data)
        {
            if (verbose)
                std::cout << "  [RL] Right-Left: комбинация (правый + левый)" << std::endl;
            // Сначала правый поворот правого поддерева (базовый примитив #1)
            node->right = rightRotate(node->right);
            // Затем левый поворот корня (базовый примитив #2)
            return leftRotate(node);
        }

        // Узел сбалансирован, возвращаем без изменений
        // После поворота баланс восстанавливается, и родительские узлы
        // обычно не требуют дополнительной балансировки
        return node;
    }

    // Поиск минимального элемента
    AVLNode* findMin(AVLNode* node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // ========================================================================
    // УДАЛЕНИЕ С БАЛАНСИРОВКОЙ
    // ========================================================================
    // ОСОБЕННОСТЬ: При удалении мы НЕ ЗНАЕМ, где был удаленный элемент
    // (он мог быть в любом месте поддерева). Поэтому для определения типа
    // поворота используется ДРУГОЙ метод - проверка баланса потомков.
    //
    // АЛГОРИТМ РАСПОЗНАВАНИЯ ПРИ УДАЛЕНИИ:
    // 1. balance > 1 => перевес слева (L)
    //    - getBalance(node->left) >= 0 => левое поддерево left выше => LL
    //    - getBalance(node->left) < 0  => правое поддерево left выше => LR
    //
    // 2. balance < -1 => перевес справа (R)
    //    - getBalance(node->right) <= 0 => правое поддерево right выше => RR
    //    - getBalance(node->right) > 0  => левое поддерево right выше => RL
    //
    // Сравнение с вставкой:
    // - При ВСТАВКЕ: используем val для определения, где новый элемент
    // - При УДАЛЕНИИ: используем баланс потомков для определения структуры
    // ========================================================================
    AVLNode* deleteHelper(AVLNode* node, int val)
    {
        // ========== ЭТАП 1: Обычное удаление из BST ==========
        if (node == nullptr)
            return node;

        if (val < node->data)
        {
            node->left = deleteHelper(node->left, val);
        }
        else if (val > node->data)
        {
            node->right = deleteHelper(node->right, val);
        }
        else
        {
            // Узел для удаления найден
            if (node->left == nullptr || node->right == nullptr)
            {
                AVLNode* temp = node->left ? node->left : node->right;

                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                AVLNode* temp = findMin(node->right);
                node->data = temp->data;
                node->right = deleteHelper(node->right, temp->data);
            }
        }

        if (node == nullptr)
            return node;

        // ========== ЭТАП 2: Проверка баланса при возврате (снизу вверх) ==========
        // Обновляем высоту текущего узла (она могла измениться после удаления)
        updateHeight(node);

        // Получаем баланс-фактор
        int balance = getBalance(node);

        // ========== ЭТАП 3: Восстановление баланса (если нужно) ==========
        // ВАЖНО: При удалении используем баланс потомков, а не val!
        
        // СЛУЧАЙ 1: Left-Left (LL)
        // Распознавание: balance > 1 (перевес слева) И getBalance(node->left) >= 0
        // (левое поддерево left выше или равно правому => структура LL)
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // СЛУЧАЙ 2: Left-Right (LR)
        // Распознавание: balance > 1 (перевес слева) И getBalance(node->left) < 0
        // (правое поддерево left выше левого => структура LR)
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // СЛУЧАЙ 3: Right-Right (RR)
        // Распознавание: balance < -1 (перевес справа) И getBalance(node->right) <= 0
        // (правое поддерево right выше или равно левому => структура RR)
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // СЛУЧАЙ 4: Right-Left (RL)
        // Распознавание: balance < -1 (перевес справа) И getBalance(node->right) > 0
        // (левое поддерево right выше правого => структура RL)
        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Обход inorder
    void inorderHelper(AVLNode* node)
    {
        if (node != nullptr)
        {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }

    // Визуализация дерева
    void printTreeHelper(AVLNode* node, int level)
    {
        if (node == nullptr)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->data << " (h=" << node->height << ", b=" << getBalance(node) << ")" << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

    // Уничтожение дерева
    void destroyTree(AVLNode* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    AVLTree(bool verboseMode = false) : root(nullptr), verbose(verboseMode) {}

    ~AVLTree()
    {
        destroyTree(root);
    }

    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        root = insertHelper(root, val);
    }

    void remove(int val)
    {
        if (verbose)
            std::cout << "Удаление " << val << ":" << std::endl;
        root = deleteHelper(root, val);
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура AVL дерева (повернуто на 90°):" << std::endl;
        std::cout << "h=высота, b=баланс" << std::endl;
        printTreeHelper(root, 0);
    }

    int getTreeHeight()
    {
        return getHeight(root);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: AVL дерево - вставка" << std::endl;
    std::cout << "========================================" << std::endl;
    
    AVLTree avl1(true);
    
    std::cout << "\n--- Вставка элементов в порядке, вызывающем дисбаланс ---" << std::endl;
    std::cout << "Вставляем: 10, 20, 30 (вызовет Right-Right поворот)" << std::endl;
    avl1.insert(10);
    avl1.insert(20);
    avl1.insert(30);
    avl1.printTree();
    
    std::cout << "\nВставляем: 5, 15 (вызовет Left-Right поворот)" << std::endl;
    avl1.insert(5);
    avl1.insert(15);
    avl1.printTree();
    
    std::cout << "\nВставляем: 25, 35" << std::endl;
    avl1.insert(25);
    avl1.insert(35);
    avl1.printTree();
    avl1.inorder();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Сравнение с обычным BST" << std::endl;
    std::cout << "========================================" << std::endl;
    
    AVLTree avl2(false);
    std::cout << "Вставка в порядке возрастания (1-7) в AVL дерево:" << std::endl;
    for (int i = 1; i <= 7; i++)
    {
        avl2.insert(i);
    }
    avl2.printTree();
    std::cout << "Высота AVL дерева: " << avl2.getTreeHeight() << std::endl;
    std::cout << "AVL дерево остается сбалансированным!" << std::endl;
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Различные случаи поворотов" << std::endl;
    std::cout << "========================================" << std::endl;
    
    AVLTree avl3(true);
    std::cout << "Тест Left-Left поворота:" << std::endl;
    avl3.insert(30);
    avl3.insert(20);
    avl3.insert(10);
    avl3.printTree();
    
    std::cout << "\nТест Right-Left поворота:" << std::endl;
    AVLTree avl4(true);
    avl4.insert(10);
    avl4.insert(30);
    avl4.insert(20);
    avl4.printTree();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 4: Ручное распознавание поворотов" << std::endl;
    std::cout << "========================================" << std::endl;
    
    AVLTree avl5(true);
    std::cout << "\nПример 1: Распознавание LL (Left-Left)" << std::endl;
    std::cout << "Вставляем: 50, 30, 20" << std::endl;
    std::cout << "Анализ:" << std::endl;
    std::cout << "  1. После вставки 20: balance(30) = 1 (перевес слева = L)" << std::endl;
    std::cout << "  2. val=20 < node->left->data=30 (новый элемент слева от left = L)" << std::endl;
    std::cout << "  3. Результат: LL => нужен один правый поворот" << std::endl;
    avl5.insert(50);
    avl5.insert(30);
    avl5.insert(20);
    avl5.printTree();
    
    std::cout << "\nПример 2: Распознавание LR (Left-Right)" << std::endl;
    AVLTree avl6(true);
    std::cout << "Вставляем: 50, 30, 40" << std::endl;
    std::cout << "Анализ:" << std::endl;
    std::cout << "  1. После вставки 40: balance(50) = 2 (перевес слева = L)" << std::endl;
    std::cout << "  2. val=40 > node->left->data=30 (новый элемент справа от left = R)" << std::endl;
    std::cout << "  3. Результат: LR => нужна комбинация (левый + правый)" << std::endl;
    avl6.insert(50);
    avl6.insert(30);
    avl6.insert(40);
    avl6.printTree();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "AVL дерево (Adelson-Velsky и Landis):" << std::endl;
    std::cout << "- Самобалансирующееся бинарное дерево поиска" << std::endl;
    std::cout << "- Баланс-фактор каждого узла: -1, 0 или 1" << std::endl;
    std::cout << "- Высота дерева: O(log n)" << std::endl;
    std::cout << "- Все операции: O(log n)" << std::endl;
    std::cout << "- Использует повороты для поддержания баланса:" << std::endl;
    std::cout << "  * Правый поворот (Right Rotate)" << std::endl;
    std::cout << "  * Левый поворот (Left Rotate)" << std::endl;
    std::cout << "  * Комбинации поворотов для сложных случаев" << std::endl;
    
    return 0;
}


