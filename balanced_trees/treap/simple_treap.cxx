#include <iostream>
#include <cstdlib>
#include <ctime>

// Узел Treap (комбинация BST и кучи)
struct TreapNode
{
    int key;           // Ключ для BST свойства
    int priority;      // Приоритет для кучи свойства
    TreapNode* left;
    TreapNode* right;
    
    TreapNode(int k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
};

// Класс Treap
class Treap
{
    TreapNode* root;
    bool verbose;

    // Разделение дерева на два по ключу
    void split(TreapNode* node, int key, TreapNode*& left, TreapNode*& right)
    {
        if (node == nullptr)
        {
            left = right = nullptr;
            return;
        }
        
        if (node->key <= key)
        {
            split(node->right, key, node->right, right);
            left = node;
        }
        else
        {
            split(node->left, key, left, node->left);
            right = node;
        }
    }

    // Объединение двух деревьев
    TreapNode* merge(TreapNode* left, TreapNode* right)
    {
        if (left == nullptr) return right;
        if (right == nullptr) return left;
        
        if (left->priority > right->priority)
        {
            left->right = merge(left->right, right);
            return left;
        }
        else
        {
            right->left = merge(left, right->left);
            return right;
        }
    }

    // Вставка с приоритетом
    TreapNode* insertHelper(TreapNode* node, int key, int priority)
    {
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Вставка узла " << key << " с приоритетом " << priority << std::endl;
            return new TreapNode(key, priority);
        }
        
        if (key <= node->key)
        {
            node->left = insertHelper(node->left, key, priority);
            // Восстанавливаем свойство кучи (max-heap)
            if (node->left->priority > node->priority)
            {
                TreapNode* temp = node;
                node = node->left;
                temp->left = node->right;
                node->right = temp;
            }
        }
        else
        {
            node->right = insertHelper(node->right, key, priority);
            // Восстанавливаем свойство кучи
            if (node->right->priority > node->priority)
            {
                TreapNode* temp = node;
                node = node->right;
                temp->right = node->left;
                node->left = temp;
            }
        }
        
        return node;
    }

    // Поиск узла
    TreapNode* searchHelper(TreapNode* node, int key)
    {
        if (node == nullptr || node->key == key)
            return node;
        
        if (key < node->key)
            return searchHelper(node->left, key);
        else
            return searchHelper(node->right, key);
    }

    // Удаление узла
    TreapNode* removeHelper(TreapNode* node, int key)
    {
        if (node == nullptr)
            return node;
        
        if (key < node->key)
            node->left = removeHelper(node->left, key);
        else if (key > node->key)
            node->right = removeHelper(node->right, key);
        else
        {
            // Узел найден, удаляем через merge
            TreapNode* temp = node;
            node = merge(node->left, node->right);
            delete temp;
        }
        
        return node;
    }

    // Обход inorder
    void inorderHelper(TreapNode* node)
    {
        if (node != nullptr)
        {
            inorderHelper(node->left);
            std::cout << node->key << "(p=" << node->priority << ") ";
            inorderHelper(node->right);
        }
    }

    // Визуализация дерева
    void printTreeHelper(TreapNode* node, int level)
    {
        if (node == nullptr)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->key << "(p=" << node->priority << ")" << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

    // Уничтожение дерева
    void destroyTree(TreapNode* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    Treap(bool verboseMode = false) : root(nullptr), verbose(verboseMode)
    {
        std::srand(std::time(nullptr));
    }

    ~Treap()
    {
        destroyTree(root);
    }

    // Вставка с автоматическим приоритетом
    void insert(int key)
    {
        int priority = std::rand();
        if (verbose)
            std::cout << "Вставка " << key << ":" << std::endl;
        root = insertHelper(root, key, priority);
    }

    // Вставка с заданным приоритетом
    void insert(int key, int priority)
    {
        if (verbose)
            std::cout << "Вставка " << key << " с приоритетом " << priority << ":" << std::endl;
        root = insertHelper(root, key, priority);
    }

    bool search(int key)
    {
        return searchHelper(root, key) != nullptr;
    }

    void remove(int key)
    {
        if (verbose)
            std::cout << "Удаление " << key << ":" << std::endl;
        root = removeHelper(root, key);
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура Treap (повернуто на 90°):" << std::endl;
        std::cout << "p=приоритет" << std::endl;
        printTreeHelper(root, 0);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ: Treap (Декартово дерево)" << std::endl;
    std::cout << "========================================" << std::endl;
    
    Treap treap(true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    treap.insert(50);
    treap.insert(30);
    treap.insert(70);
    treap.insert(20);
    treap.insert(40);
    treap.insert(60);
    treap.insert(80);
    
    treap.printTree();
    treap.inorder();
    
    std::cout << "\n--- Поиск ---" << std::endl;
    std::cout << "Поиск 40: " << (treap.search(40) ? "найден" : "не найден") << std::endl;
    std::cout << "Поиск 90: " << (treap.search(90) ? "найден" : "не найден") << std::endl;
    
    std::cout << "\n--- Удаление ---" << std::endl;
    treap.remove(40);
    treap.printTree();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Treap (Декартово дерево):" << std::endl;
    std::cout << "- Комбинация BST и кучи" << std::endl;
    std::cout << "- Балансировка через случайные приоритеты" << std::endl;
    std::cout << "- Сложность: O(log n) в среднем" << std::endl;
    std::cout << "- Простая реализация без parent-указателей" << std::endl;
    
    return 0;
}

