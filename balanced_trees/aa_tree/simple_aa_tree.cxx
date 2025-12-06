#include <iostream>

// Узел AA дерева
struct AANode
{
    int data;
    int level;      // Уровень узла (вместо цвета в RB-дереве)
    AANode* left;
    AANode* right;
    
    AANode(int val) : data(val), level(1), left(nullptr), right(nullptr) {}
};

// Класс AA дерева
class AATree
{
    AANode* root;
    AANode* nil;    // Специальный узел-лист
    bool verbose;

    // Операция skew (выравнивание) - аналог правого поворота
    AANode* skew(AANode* node)
    {
        if (node == nil || node->left == nil)
            return node;
        
        if (node->left->level == node->level)
        {
            if (verbose)
                std::cout << "  Skew вокруг " << node->data << std::endl;
            
            AANode* temp = node->left;
            node->left = temp->right;
            temp->right = node;
            return temp;
        }
        return node;
    }

    // Операция split (разделение) - аналог левого поворота с повышением уровня
    AANode* split(AANode* node)
    {
        if (node == nil || node->right == nil || node->right->right == nil)
            return node;
        
        if (node->level == node->right->right->level)
        {
            if (verbose)
                std::cout << "  Split вокруг " << node->data << std::endl;
            
            AANode* temp = node->right;
            node->right = temp->left;
            temp->left = node;
            temp->level++;
            return temp;
        }
        return node;
    }

    // Вставка
    AANode* insertHelper(AANode* node, int val)
    {
        if (node == nil)
        {
            if (verbose)
                std::cout << "  Вставка узла " << val << std::endl;
            return new AANode(val);
        }
        
        if (val < node->data)
            node->left = insertHelper(node->left, val);
        else if (val > node->data)
            node->right = insertHelper(node->right, val);
        else
            return node;  // Дубликаты не допускаются
        
        // Применяем skew и split для балансировки
        node = skew(node);
        node = split(node);
        
        return node;
    }

    // Поиск
    AANode* searchHelper(AANode* node, int val)
    {
        if (node == nil || node->data == val)
            return node;
        
        if (val < node->data)
            return searchHelper(node->left, val);
        else
            return searchHelper(node->right, val);
    }

    // Удаление
    AANode* removeHelper(AANode* node, int val)
    {
        if (node == nil)
            return node;
        
        if (val < node->data)
            node->left = removeHelper(node->left, val);
        else if (val > node->data)
            node->right = removeHelper(node->right, val);
        else
        {
            if (node->left == nil && node->right == nil)
            {
                delete node;
                return nil;
            }
            else if (node->left == nil)
            {
                AANode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nil)
            {
                AANode* temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                // Находим преемника
                AANode* successor = node->right;
                while (successor->left != nil)
                    successor = successor->left;
                
                node->data = successor->data;
                node->right = removeHelper(node->right, successor->data);
            }
        }
        
        // Восстанавливаем баланс
        if (node->left->level < node->level - 1 || node->right->level < node->level - 1)
        {
            node->level--;
            if (node->right->level > node->level)
                node->right->level = node->level;
            
            node = skew(node);
            node->right = skew(node->right);
            node->right->right = skew(node->right->right);
            node = split(node);
            node->right = split(node->right);
        }
        
        return node;
    }

    // Обход inorder
    void inorderHelper(AANode* node)
    {
        if (node != nil)
        {
            inorderHelper(node->left);
            std::cout << node->data << "(L=" << node->level << ") ";
            inorderHelper(node->right);
        }
    }

    // Визуализация
    void printTreeHelper(AANode* node, int level)
    {
        if (node == nil)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->data << "(L=" << node->level << ")" << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

    // Уничтожение дерева
    void destroyTree(AANode* node)
    {
        if (node != nil)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    AATree(bool verboseMode = false) : verbose(verboseMode)
    {
        nil = new AANode(0);
        nil->level = 0;
        root = nil;
    }

    ~AATree()
    {
        destroyTree(root);
        delete nil;
    }

    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        root = insertHelper(root, val);
    }

    bool search(int val)
    {
        return searchHelper(root, val) != nil;
    }

    void remove(int val)
    {
        if (verbose)
            std::cout << "Удаление " << val << ":" << std::endl;
        root = removeHelper(root, val);
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура AA дерева (повернуто на 90°):" << std::endl;
        std::cout << "L=уровень" << std::endl;
        printTreeHelper(root, 0);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ: AA Tree" << std::endl;
    std::cout << "========================================" << std::endl;
    
    AATree aa(true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    aa.insert(50);
    aa.insert(30);
    aa.insert(70);
    aa.insert(20);
    aa.insert(40);
    aa.insert(60);
    aa.insert(80);
    
    aa.printTree();
    aa.inorder();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "AA Tree (Arne Andersson Tree):" << std::endl;
    std::cout << "- Упрощенная версия красно-черного дерева" << std::endl;
    std::cout << "- Использует уровни вместо цветов" << std::endl;
    std::cout << "- Операции: skew и split" << std::endl;
    std::cout << "- Сложность: O(log n)" << std::endl;
    
    return 0;
}

