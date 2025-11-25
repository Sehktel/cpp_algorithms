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

    // Правый поворот (для случая левого перевеса)
    AVLNode* rightRotate(AVLNode* y)
    {
        if (verbose)
            std::cout << "  Правый поворот вокруг " << y->data << std::endl;
        
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Выполняем поворот
        x->right = y;
        y->left = T2;

        // Обновляем высоты
        updateHeight(y);
        updateHeight(x);

        return x; // Новый корень
    }

    // Левый поворот (для случая правого перевеса)
    AVLNode* leftRotate(AVLNode* x)
    {
        if (verbose)
            std::cout << "  Левый поворот вокруг " << x->data << std::endl;
        
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Выполняем поворот
        y->left = x;
        x->right = T2;

        // Обновляем высоты
        updateHeight(x);
        updateHeight(y);

        return y; // Новый корень
    }

    // Вставка с балансировкой
    AVLNode* insertHelper(AVLNode* node, int val)
    {
        // 1. Обычная вставка в BST
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Создаем новый узел: " << val << std::endl;
            return new AVLNode(val);
        }

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

        // 2. Обновляем высоту текущего узла
        updateHeight(node);

        // 3. Получаем баланс-фактор
        int balance = getBalance(node);

        if (verbose && balance != 0)
        {
            std::cout << "  Баланс узла " << node->data << ": " << balance << std::endl;
        }

        // 4. Если узел несбалансирован, выполняем повороты

        // Случай Left Left (левое поддерево левого потомка перевешивает)
        if (balance > 1 && val < node->left->data)
        {
            if (verbose)
                std::cout << "  Случай Left-Left, нужен правый поворот" << std::endl;
            return rightRotate(node);
        }

        // Случай Right Right (правое поддерево правого потомка перевешивает)
        if (balance < -1 && val > node->right->data)
        {
            if (verbose)
                std::cout << "  Случай Right-Right, нужен левый поворот" << std::endl;
            return leftRotate(node);
        }

        // Случай Left Right (правое поддерево левого потомка перевешивает)
        if (balance > 1 && val > node->left->data)
        {
            if (verbose)
                std::cout << "  Случай Left-Right, нужен левый-правый поворот" << std::endl;
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Случай Right Left (левое поддерево правого потомка перевешивает)
        if (balance < -1 && val < node->right->data)
        {
            if (verbose)
                std::cout << "  Случай Right-Left, нужен правый-левый поворот" << std::endl;
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Узел сбалансирован, возвращаем без изменений
        return node;
    }

    // Поиск минимального элемента
    AVLNode* findMin(AVLNode* node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // Удаление с балансировкой
    AVLNode* deleteHelper(AVLNode* node, int val)
    {
        // 1. Обычное удаление из BST
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

        // 2. Обновляем высоту
        updateHeight(node);

        // 3. Получаем баланс-фактор
        int balance = getBalance(node);

        // 4. Если узел несбалансирован, выполняем повороты
        // Left Left
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left
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


