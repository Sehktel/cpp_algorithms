#include <iostream>
#include <queue>

// Узел бинарного дерева поиска
struct TreeNode
{
    int data;
    TreeNode* left;   // Левый потомок
    TreeNode* right;  // Правый потомок
    
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Класс бинарного дерева поиска (BST)
class BinarySearchTree
{
    TreeNode* root;
    bool verbose;

    // Вспомогательная функция для рекурсивного удаления
    void destroyTree(TreeNode* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Вспомогательная функция для вывода дерева (inorder)
    void inorderHelper(TreeNode* node)
    {
        if (node != nullptr)
        {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }

    // Вспомогательная функция для вставки
    TreeNode* insertHelper(TreeNode* node, int val)
    {
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Создаем новый узел: " << val << std::endl;
            return new TreeNode(val);
        }

        if (verbose)
            std::cout << "  Сравниваем " << val << " с " << node->data;

        if (val < node->data)
        {
            if (verbose)
                std::cout << " -> идем влево" << std::endl;
            node->left = insertHelper(node->left, val);
        }
        else if (val > node->data)
        {
            if (verbose)
                std::cout << " -> идем вправо" << std::endl;
            node->right = insertHelper(node->right, val);
        }
        else
        {
            if (verbose)
                std::cout << " -> значение уже существует" << std::endl;
        }

        return node;
    }

    // Вспомогательная функция для поиска
    TreeNode* searchHelper(TreeNode* node, int val)
    {
        if (node == nullptr || node->data == val)
        {
            return node;
        }

        if (verbose)
            std::cout << "  Сравниваем " << val << " с " << node->data;

        if (val < node->data)
        {
            if (verbose)
                std::cout << " -> идем влево" << std::endl;
            return searchHelper(node->left, val);
        }
        else
        {
            if (verbose)
                std::cout << " -> идем вправо" << std::endl;
            return searchHelper(node->right, val);
        }
    }

    // Вспомогательная функция для поиска минимального элемента
    TreeNode* findMin(TreeNode* node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // Вспомогательная функция для удаления
    TreeNode* deleteHelper(TreeNode* node, int val)
    {
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
            // Узел найден, нужно удалить
            if (verbose)
                std::cout << "  Найден узел для удаления: " << val << std::endl;

            // Случай 1: нет потомков или только один потомок
            if (node->left == nullptr)
            {
                TreeNode* temp = node->right;
                delete node;
                if (verbose)
                    std::cout << "  Удаляем узел (нет левого потомка)" << std::endl;
                return temp;
            }
            else if (node->right == nullptr)
            {
                TreeNode* temp = node->left;
                delete node;
                if (verbose)
                    std::cout << "  Удаляем узел (нет правого потомка)" << std::endl;
                return temp;
            }

            // Случай 2: два потомка
            // Находим минимальный элемент в правом поддереве
            TreeNode* temp = findMin(node->right);
            if (verbose)
                std::cout << "  Заменяем на минимальный в правом поддереве: " << temp->data << std::endl;
            node->data = temp->data;
            node->right = deleteHelper(node->right, temp->data);
        }

        return node;
    }

    // Высота дерева
    int heightHelper(TreeNode* node)
    {
        if (node == nullptr)
            return -1;
        return 1 + std::max(heightHelper(node->left), heightHelper(node->right));
    }

    // Визуализация дерева (level-order)
    void printTreeHelper(TreeNode* node, int level)
    {
        if (node == nullptr)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->data << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

public:
    BinarySearchTree(bool verboseMode = false) : root(nullptr), verbose(verboseMode) {}

    ~BinarySearchTree()
    {
        destroyTree(root);
    }

    // Вставка элемента
    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        root = insertHelper(root, val);
    }

    // Поиск элемента
    bool search(int val)
    {
        if (verbose)
            std::cout << "Поиск " << val << ":" << std::endl;
        TreeNode* result = searchHelper(root, val);
        if (result != nullptr)
        {
            if (verbose)
                std::cout << "  Найдено!" << std::endl;
            return true;
        }
        else
        {
            if (verbose)
                std::cout << "  Не найдено" << std::endl;
            return false;
        }
    }

    // Удаление элемента
    void remove(int val)
    {
        if (verbose)
            std::cout << "Удаление " << val << ":" << std::endl;
        root = deleteHelper(root, val);
    }

    // Обход в порядке возрастания (inorder)
    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    // Высота дерева
    int height()
    {
        return heightHelper(root);
    }

    // Визуализация дерева
    void printTree()
    {
        std::cout << "\nСтруктура дерева (повернуто на 90°):" << std::endl;
        printTreeHelper(root, 0);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Базовые операции BST" << std::endl;
    std::cout << "========================================" << std::endl;
    
    BinarySearchTree bst1(true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    bst1.insert(50);
    bst1.insert(30);
    bst1.insert(70);
    bst1.insert(20);
    bst1.insert(40);
    bst1.insert(60);
    bst1.insert(80);
    
    bst1.printTree();
    bst1.inorder();
    std::cout << "Высота дерева: " << bst1.height() << std::endl;
    
    std::cout << "\n--- Поиск элементов ---" << std::endl;
    bst1.search(40);
    bst1.search(90);
    
    std::cout << "\n--- Удаление элементов ---" << std::endl;
    bst1.remove(20);
    bst1.printTree();
    bst1.remove(30);
    bst1.printTree();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Несбалансированное дерево" << std::endl;
    std::cout << "========================================" << std::endl;
    
    BinarySearchTree bst2(false);
    // Вставляем в порядке возрастания - получаем вырожденное дерево
    std::cout << "Вставка в порядке возрастания (1, 2, 3, 4, 5, 6, 7):" << std::endl;
    for (int i = 1; i <= 7; i++)
    {
        bst2.insert(i);
    }
    
    bst2.printTree();
    std::cout << "Высота дерева: " << bst2.height() << std::endl;
    std::cout << "Проблема: дерево вырождается в список!" << std::endl;
    std::cout << "Решение: использовать самобалансирующиеся деревья (AVL, RB-tree)" << std::endl;
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Бинарное дерево поиска (BST):" << std::endl;
    std::cout << "- Левое поддерево < корень < правое поддерево" << std::endl;
    std::cout << "- Вставка, поиск, удаление: O(h), где h - высота" << std::endl;
    std::cout << "- В худшем случае (вырожденное дерево): O(n)" << std::endl;
    std::cout << "- В лучшем случае (сбалансированное): O(log n)" << std::endl;
    
    return 0;
}


