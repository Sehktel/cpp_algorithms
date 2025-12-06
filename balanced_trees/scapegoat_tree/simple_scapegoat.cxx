#include <iostream>
#include <vector>

// Узел Scapegoat дерева
struct ScapegoatNode
{
    int data;
    ScapegoatNode* left;
    ScapegoatNode* right;
    
    ScapegoatNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Класс Scapegoat дерева
class ScapegoatTree
{
    ScapegoatNode* root;
    double alpha;      // Параметр балансировки (0.5 < alpha < 1)
    bool verbose;

    // Подсчет размера поддерева
    int size(ScapegoatNode* node)
    {
        if (node == nullptr)
            return 0;
        return 1 + size(node->left) + size(node->right);
    }

    // Проверка, нужно ли перестраивать
    bool needsRebuild(ScapegoatNode* node)
    {
        if (node == nullptr)
            return false;
        
        int nodeSize = size(node);
        int leftSize = size(node->left);
        int rightSize = size(node->right);
        
        return leftSize > alpha * nodeSize || rightSize > alpha * nodeSize;
    }

    // Сбор узлов в массив (inorder)
    void flatten(ScapegoatNode* node, std::vector<ScapegoatNode*>& nodes)
    {
        if (node == nullptr)
            return;
        
        flatten(node->left, nodes);
        nodes.push_back(node);
        flatten(node->right, nodes);
    }

    // Построение сбалансированного дерева из массива
    ScapegoatNode* buildTree(std::vector<ScapegoatNode*>& nodes, int start, int end)
    {
        if (start > end)
            return nullptr;
        
        int mid = (start + end) / 2;
        ScapegoatNode* node = nodes[mid];
        
        node->left = buildTree(nodes, start, mid - 1);
        node->right = buildTree(nodes, mid + 1, end);
        
        return node;
    }

    // Перестройка поддерева
    ScapegoatNode* rebuild(ScapegoatNode* node)
    {
        if (node == nullptr)
            return nullptr;
        
        if (verbose)
            std::cout << "  Перестройка поддерева с корнем " << node->data << std::endl;
        
        std::vector<ScapegoatNode*> nodes;
        flatten(node, nodes);
        
        return buildTree(nodes, 0, nodes.size() - 1);
    }

    // Вставка
    ScapegoatNode* insertHelper(ScapegoatNode* node, int val, int depth)
    {
        if (node == nullptr)
        {
            if (verbose)
                std::cout << "  Вставка узла " << val << " на глубине " << depth << std::endl;
            return new ScapegoatNode(val);
        }
        
        if (val < node->data)
            node->left = insertHelper(node->left, val, depth + 1);
        else if (val > node->data)
            node->right = insertHelper(node->right, val, depth + 1);
        else
            return node;  // Дубликаты не допускаются
        
        // Проверяем, нужно ли перестраивать
        if (needsRebuild(node))
        {
            if (verbose)
                std::cout << "  Обнаружен дисбаланс в узле " << node->data << ", перестраиваем" << std::endl;
            node = rebuild(node);
        }
        
        return node;
    }

    // Поиск
    ScapegoatNode* searchHelper(ScapegoatNode* node, int val)
    {
        if (node == nullptr || node->data == val)
            return node;
        
        if (val < node->data)
            return searchHelper(node->left, val);
        else
            return searchHelper(node->right, val);
    }

    // Обход inorder
    void inorderHelper(ScapegoatNode* node)
    {
        if (node != nullptr)
        {
            inorderHelper(node->left);
            std::cout << node->data << " ";
            inorderHelper(node->right);
        }
    }

    // Визуализация
    void printTreeHelper(ScapegoatNode* node, int level)
    {
        if (node == nullptr)
            return;

        printTreeHelper(node->right, level + 1);
        
        for (int i = 0; i < level; i++)
            std::cout << "    ";
        std::cout << node->data << std::endl;
        
        printTreeHelper(node->left, level + 1);
    }

    // Уничтожение дерева
    void destroyTree(ScapegoatNode* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    ScapegoatTree(double alphaValue = 0.67, bool verboseMode = false) 
        : root(nullptr), alpha(alphaValue), verbose(verboseMode) {}

    ~ScapegoatTree()
    {
        destroyTree(root);
    }

    void insert(int val)
    {
        if (verbose)
            std::cout << "Вставка " << val << ":" << std::endl;
        root = insertHelper(root, val, 0);
    }

    bool search(int val)
    {
        return searchHelper(root, val) != nullptr;
    }

    void inorder()
    {
        std::cout << "Inorder обход: ";
        inorderHelper(root);
        std::cout << std::endl;
    }

    void printTree()
    {
        std::cout << "\nСтруктура Scapegoat дерева (повернуто на 90°):" << std::endl;
        printTreeHelper(root, 0);
    }
};

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  ТЕСТ: Scapegoat Tree" << std::endl;
    std::cout << "========================================" << std::endl;
    
    ScapegoatTree scapegoat(0.67, true);
    
    std::cout << "\n--- Вставка элементов ---" << std::endl;
    scapegoat.insert(50);
    scapegoat.insert(30);
    scapegoat.insert(70);
    scapegoat.insert(20);
    scapegoat.insert(40);
    scapegoat.insert(60);
    scapegoat.insert(80);
    
    scapegoat.printTree();
    scapegoat.inorder();
    
    std::cout << "\n\n=== ВЫВОД ===" << std::endl;
    std::cout << "Scapegoat Tree:" << std::endl;
    std::cout << "- Перестраивает поддерево при дисбалансе" << std::endl;
    std::cout << "- Не требует parent-указателей" << std::endl;
    std::cout << "- Сложность: O(log n) амортизированно" << std::endl;
    std::cout << "- Параметр alpha: 0.67" << std::endl;
    
    return 0;
}

