#include <iostream>

// Цвет узла в красно-черном дереве
enum Color { RED, BLACK };

// Узел красно-черного дерева
struct RBNode
{
    int data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    
    RBNode(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Класс красно-черного дерева
class RedBlackTree
{
    RBNode* root;
    RBNode* nil;  // Специальный узел-лист (черный)
    bool verbose;

    // Вспомогательная функция для вывода цвета
    std::string getColorName(Color c)
    {
        return (c == RED) ? "КРАСНЫЙ" : "ЧЕРНЫЙ";
    }

    // Левый поворот
    void leftRotate(RBNode* x)
    {
        if (verbose)
            std::cout << "  Левый поворот вокруг " << x->data << std::endl;
        
        RBNode* y = x->right;
        x->right = y->left;
        
        if (y->left != nil)
            y->left->parent = x;
        
        y->parent = x->parent;
        
        if (x->parent == nil)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->left = x;
        x->parent = y;
    }

    // Правый поворот
    void rightRotate(RBNode* y)
    {
        if (verbose)
            std::cout << "  Правый поворот вокруг " << y->data << std::endl;
        
        RBNode* x = y->left;
        y->left = x->right;
        
        if (x->right != nil)
            x->right->parent = y;
        
        x->parent = y->parent;
        
        if (y->parent == nil)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        
        x->right = y;
        y->parent = x;
    }

    // Исправление нарушений после вставки
    void insertFixup(RBNode* z)
    {
        while (z->parent->color == RED)
        {
            if (z->parent == z->parent->parent->left)
            {
                RBNode* y = z->parent->parent->right;
                
                // Случай 1: дядя красный
                if (y->color == RED)
                {
                    if (verbose)
                        std::cout << "  Случай 1: дядя красный, перекрашиваем" << std::endl;
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    // Случай 2: дядя черный, z - правый потомок
                    if (z == z->parent->right)
                    {
                        if (verbose)
                            std::cout << "  Случай 2: дядя черный, z правый потомок" << std::endl;
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Случай 3: дядя черный, z - левый потомок
                    if (verbose)
                        std::cout << "  Случай 3: дядя черный, z левый потомок" << std::endl;
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                // Симметричный случай (правая сторона)
                RBNode* y = z->parent->parent->left;
                
                if (y->color == RED)
                {
                    if (verbose)
                        std::cout << "  Случай 1 (симметричный): дядя красный" << std::endl;
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        if (verbose)
                            std::cout << "  Случай 2 (симметричный): дядя черный, z левый потомок" << std::endl;
                        z = z->parent;
                        rightRotate(z);
                    }
                    if (verbose)
                        std::cout << "  Случай 3 (симметричный): дядя черный, z правый потомок" << std::endl;
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Вставка узла
    void insertHelper(RBNode* z)
    {
        RBNode* y = nil;
        RBNode* x = root;
        
        // Находим место для вставки (как в обычном BST)
        while (x != nil)
        {
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }
        
        z->parent = y;
        if (y == nil)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;
        
        z->left = nil;
        z->right = nil;
        z->color = RED;
        
        if (verbose)
            std::cout << "  Вставлен узел " << z->data << " (красный)" << std::endl;
        
        // Исправляем возможные нарушения свойств красно-черного дерева
        insertFixup(z);
    }

    // Обход inorder
    void inorderHelper(RBNode* node)
    {
        if (node != nil)
        {
            inorderHelper(node->left);
            std::cout << node->data << "(" << getColorName(node->color) << ") ";
            inorderHelper(node->right);
        }
    }

    // Визуализация дерева
    void printTreeHelper(RBNode* node, int level, const std::string& prefix)
    {
        if (node != nil)
        {
            printTreeHelper(node->right, level + 1, prefix + "    ");
            
            std::cout << prefix;
            std::cout << node->data;
            if (node->color == RED)
                std::cout << "(R)";
            else
                std::cout << "(B)";
            std::cout << std::endl;
            
            printTreeHelper(node->left, level + 1, prefix + "    ");
        }
    }

    // Уничтожение дерева
    void destroyTree(RBNode* node)
    {
        if (node != nil)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    RedBlackTree(bool verboseMode = false) : verbose(verboseMode)
    {
        nil = new RBNode(0);
        nil->color = BLACK;
        root = nil;
    }

    ~RedBlackTree()
    {
        destroyTree(root);
        delete nil;
    }

    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        RBNode* z = new RBNode(val);
        insertHelper(z);
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура красно-черного дерева (повернуто на 90°):" << std::endl;
        std::cout << "R=красный, B=черный" << std::endl;
        printTreeHelper(root, 0, "");
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ 1: Красно-черное дерево - вставка" << std::endl;
    std::cout << "========================================" << std::endl;
    
    RedBlackTree rbt1(true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    rbt1.insert(10);
    rbt1.printTree();
    
    rbt1.insert(20);
    rbt1.printTree();
    
    rbt1.insert(30);
    rbt1.printTree();
    
    rbt1.insert(5);
    rbt1.printTree();
    
    rbt1.insert(15);
    rbt1.printTree();
    
    rbt1.insert(25);
    rbt1.insert(35);
    rbt1.printTree();
    rbt1.inorder();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 2: Вставка в порядке возрастания" << std::endl;
    std::cout << "========================================" << std::endl;
    
    RedBlackTree rbt2(false);
    std::cout << "Вставка в порядке возрастания (1-7) в красно-черное дерево:" << std::endl;
    for (int i = 1; i <= 7; i++)
    {
        rbt2.insert(i);
    }
    rbt2.printTree();
    rbt2.inorder();
    
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "  ТЕСТ 3: Большое дерево" << std::endl;
    std::cout << "========================================" << std::endl;
    
    RedBlackTree rbt3(false);
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    std::cout << "Вставка множества элементов:" << std::endl;
    for (int val : values)
    {
        rbt3.insert(val);
    }
    rbt3.printTree();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Красно-черное дерево (Red-Black Tree):" << std::endl;
    std::cout << "- Самобалансирующееся бинарное дерево поиска" << std::endl;
    std::cout << "- Свойства красно-черного дерева:" << std::endl;
    std::cout << "  1. Каждый узел либо красный, либо черный" << std::endl;
    std::cout << "  2. Корень всегда черный" << std::endl;
    std::cout << "  3. Листья (nil) черные" << std::endl;
    std::cout << "  4. Если узел красный, оба его потомка черные" << std::endl;
    std::cout << "  5. Все пути от узла до листьев содержат одинаковое число черных узлов" << std::endl;
    std::cout << "- Высота дерева: O(log n)" << std::endl;
    std::cout << "- Все операции: O(log n)" << std::endl;
    std::cout << "- Используется в std::map и std::set в C++" << std::endl;
    std::cout << "- Меньше поворотов, чем AVL, но может быть менее сбалансированным" << std::endl;
    
    return 0;
}


