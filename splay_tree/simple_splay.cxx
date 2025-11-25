#include <iostream>

// Узел Splay дерева
struct SplayNode
{
    int data;
    SplayNode* left;
    SplayNode* right;
    SplayNode* parent;
    
    SplayNode(int val) : data(val), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Класс Splay дерева
// Особенность: после каждой операции найденный узел "поднимается" в корень
class SplayTree
{
    SplayNode* root;
    bool verbose;

    // Вспомогательная функция для вывода пути
    void printPath(const std::string& operation)
    {
        if (verbose)
            std::cout << "  " << operation << std::endl;
    }

    // Правый поворот (Zig)
    SplayNode* rightRotate(SplayNode* x)
    {
        if (verbose)
            std::cout << "  Правый поворот (Zig) вокруг " << x->data << std::endl;
        
        SplayNode* y = x->left;
        x->left = y->right;
        
        if (y->right != nullptr)
            y->right->parent = x;
        
        y->parent = x->parent;
        
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        
        y->right = x;
        x->parent = y;
        
        return y;
    }

    // Левый поворот (Zag)
    SplayNode* leftRotate(SplayNode* x)
    {
        if (verbose)
            std::cout << "  Левый поворот (Zag) вокруг " << x->data << std::endl;
        
        SplayNode* y = x->right;
        x->right = y->left;
        
        if (y->left != nullptr)
            y->left->parent = x;
        
        y->parent = x->parent;
        
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->left = x;
        x->parent = y;
        
        return y;
    }

    // Splay операция: поднимает узел x в корень дерева
    void splay(SplayNode* x)
    {
        while (x->parent != nullptr)
        {
            if (x->parent->parent == nullptr)
            {
                // Случай Zig или Zag (один поворот)
                if (x->parent->left == x)
                {
                    printPath("Zig: x - левый потомок корня");
                    rightRotate(x->parent);
                }
                else
                {
                    printPath("Zag: x - правый потомок корня");
                    leftRotate(x->parent);
                }
            }
            else if (x->parent->left == x && x->parent->parent->left == x->parent)
            {
                // Случай Zig-Zig (два правых поворота)
                printPath("Zig-Zig: x и родитель - левые потомки");
                rightRotate(x->parent->parent);
                rightRotate(x->parent);
            }
            else if (x->parent->right == x && x->parent->parent->right == x->parent)
            {
                // Случай Zag-Zag (два левых поворота)
                printPath("Zag-Zag: x и родитель - правые потомки");
                leftRotate(x->parent->parent);
                leftRotate(x->parent);
            }
            else if (x->parent->right == x && x->parent->parent->left == x->parent)
            {
                // Случай Zig-Zag (правый, затем левый поворот)
                printPath("Zig-Zag: x правый, родитель левый");
                leftRotate(x->parent);
                rightRotate(x->parent);
            }
            else
            {
                // Случай Zag-Zig (левый, затем правый поворот)
                printPath("Zag-Zig: x левый, родитель правый");
                rightRotate(x->parent);
                leftRotate(x->parent);
            }
        }
    }

    // Вставка с последующим splay
    SplayNode* insertHelper(SplayNode* node, SplayNode* parent, int val)
    {
        if (node == nullptr)
        {
            SplayNode* newNode = new SplayNode(val);
            newNode->parent = parent;
            if (verbose)
                std::cout << "  Создан новый узел: " << val << std::endl;
            return newNode;
        }

        if (val < node->data)
        {
            node->left = insertHelper(node->left, node, val);
        }
        else if (val > node->data)
        {
            node->right = insertHelper(node->right, node, val);
        }
        else
        {
            // Дубликат - просто делаем splay
            if (verbose)
                std::cout << "  Значение уже существует: " << val << std::endl;
        }

        return node;
    }

    // Поиск с последующим splay
    SplayNode* searchHelper(SplayNode* node, int val)
    {
        if (node == nullptr || node->data == val)
            return node;

        if (val < node->data)
            return searchHelper(node->left, val);
        else
            return searchHelper(node->right, val);
    }

    // Поиск максимального элемента в поддереве
    SplayNode* findMax(SplayNode* node)
    {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    // Обход inorder
    void inorderHelper(SplayNode* node)
    {
        if (node != nullptr)
        {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }

    // Визуализация дерева
    void printTreeHelper(SplayNode* node, int level)
    {
        if (node == nullptr)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->data;
        if (node == root)
            std::cout << " (корень)";
        std::cout << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

    // Уничтожение дерева
    void destroyTree(SplayNode* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    SplayTree(bool verboseMode = false) : root(nullptr), verbose(verboseMode) {}

    ~SplayTree()
    {
        destroyTree(root);
    }

    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        
        root = insertHelper(root, nullptr, val);
        
        // Находим вставленный узел и делаем splay
        SplayNode* inserted = searchHelper(root, val);
        if (inserted != nullptr)
        {
            splay(inserted);
            root = inserted;
        }
    }

    bool search(int val)
    {
        if (verbose)
            std::cout << "Поиск " << val << ":" << std::endl;
        
        SplayNode* found = searchHelper(root, val);
        
        if (found != nullptr)
        {
            splay(found);
            root = found;
            if (verbose)
                std::cout << "  Найдено! Узел поднят в корень." << std::endl;
            return true;
        }
        else
        {
            if (verbose)
                std::cout << "  Не найдено" << std::endl;
            return false;
        }
    }

    void remove(int val)
    {
        if (verbose)
            std::cout << "Удаление " << val << ":" << std::endl;
        
        if (root == nullptr)
            return;

        // Ищем узел и поднимаем его в корень
        SplayNode* node = searchHelper(root, val);
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Узел не найден" << std::endl;
            return;
        }

        splay(node);
        root = node;

        if (root->left == nullptr)
        {
            // Нет левого поддерева
            root = root->right;
            if (root != nullptr)
                root->parent = nullptr;
            delete node;
        }
        else if (root->right == nullptr)
        {
            // Нет правого поддерева
            root = root->left;
            if (root != nullptr)
                root->parent = nullptr;
            delete node;
        }
        else
        {
            // Есть оба поддерева
            // Находим максимум в левом поддереве
            SplayNode* maxLeft = findMax(root->left);
            splay(maxLeft);
            maxLeft->right = root->right;
            if (root->right != nullptr)
                root->right->parent = maxLeft;
            root = maxLeft;
            delete node;
        }
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура Splay дерева (повернуто на 90°):" << std::endl;
        printTreeHelper(root, 0);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Splay дерево - базовые операции" << std::endl;
    std::cout << "========================================" << std::endl;
    
    SplayTree splay1(true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    splay1.insert(50);
    splay1.printTree();
    
    splay1.insert(30);
    splay1.printTree();
    
    splay1.insert(70);
    splay1.printTree();
    
    splay1.insert(20);
    splay1.printTree();
    
    splay1.insert(40);
    splay1.printTree();
    
    splay1.inorder();
    
    std::cout << "\n--- Поиск элементов (после поиска узел поднимается в корень) ---" << std::endl;
    splay1.search(20);
    splay1.printTree();
    
    splay1.search(40);
    splay1.printTree();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Локальность доступа" << std::endl;
    std::cout << "========================================" << std::endl;
    
    SplayTree splay2(false);
    for (int i = 1; i <= 10; i++)
    {
        splay2.insert(i);
    }
    
    std::cout << "Исходное дерево:" << std::endl;
    splay2.printTree();
    
    std::cout << "\nИщем элемент 5 (он поднимется в корень):" << std::endl;
    splay2.search(5);
    splay2.printTree();
    
    std::cout << "\nИщем элемент 5 снова (он уже в корне, дерево не изменится):" << std::endl;
    splay2.search(5);
    splay2.printTree();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Различные случаи поворотов" << std::endl;
    std::cout << "========================================" << std::endl;
    
    SplayTree splay3(true);
    std::cout << "Тест Zig-Zig (последовательные левые потомки):" << std::endl;
    splay3.insert(30);
    splay3.insert(20);
    splay3.insert(10);
    splay3.printTree();
    
    std::cout << "\nТест Zig-Zag:" << std::endl;
    SplayTree splay4(true);
    splay4.insert(10);
    splay4.insert(30);
    splay4.insert(20);
    splay4.printTree();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Splay дерево:" << std::endl;
    std::cout << "- Самобалансирующееся бинарное дерево поиска" << std::endl;
    std::cout << "- После каждой операции найденный узел 'поднимается' в корень" << std::endl;
    std::cout << "- Использует операцию 'splay' для перемещения узла в корень" << std::endl;
    std::cout << "- Типы поворотов:" << std::endl;
    std::cout << "  * Zig (один правый поворот)" << std::endl;
    std::cout << "  * Zag (один левый поворот)" << std::endl;
    std::cout << "  * Zig-Zig (два правых поворота)" << std::endl;
    std::cout << "  * Zag-Zag (два левых поворота)" << std::endl;
    std::cout << "  * Zig-Zag (правый, затем левый)" << std::endl;
    std::cout << "  * Zag-Zig (левый, затем правый)" << std::endl;
    std::cout << "- Амортизированная сложность: O(log n)" << std::endl;
    std::cout << "- Хорошо работает при локальности доступа (часто используемые элементы)" << std::endl;
    std::cout << "- Не гарантирует сбалансированность в каждый момент времени" << std::endl;
    
    return 0;
}


