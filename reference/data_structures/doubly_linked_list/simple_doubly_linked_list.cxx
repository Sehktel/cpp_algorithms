#include <iostream>

// Определяем структуру узла двунаправленного списка
struct node
{
    int data;
    node *next; // Указатель на следующий элемент
    node *prev; // Указатель на предыдущий элемент
};

// Определяем класс двунаправленный список
class doubly_linked_list
{
    // Указатели на начало и конец списка
    node *head; // Указатель на первый элемент
    node *tail; // Указатель на последний элемент

    // публичные методы
    public:
        // Конструктор объекта списка
        doubly_linked_list() 
        {
            head = NULL; // При создании списка начало указывает в никуда
            tail = NULL; // При создании списка конец тоже указывает в никуда
        }
        void insert_front(int a);  // Добавить элемент в начало списка
        void insert_back(int a);   // Добавить элемент в конец списка
        void delete_front();       // Удалить элемент из начала списка
        void delete_back();        // Удалить элемент из конца списка
        void display_forward();    // Отобразить список от начала к концу
        void display_backward();   // Отобразить список от конца к началу
        ~doubly_linked_list();     // Деструктор объекта
};

void doubly_linked_list::insert_front(int a)
{
    node *temp;
    temp = new node;
    std::cout << "Inserted at front : " << a << std::endl;
    temp->data = a;
    temp->prev = NULL; // Элемент в начале не имеет предыдущего
    temp->next = head; // Следующий элемент - это старый head
    
    // Если список пуст, новый элемент становится и первым, и последним
    if (head == NULL)
    {
        head = temp;
        tail = temp;
    }
    else
    {
        // Иначе обновляем связь со старым первым элементом
        head->prev = temp;
        head = temp;
    }
}

void doubly_linked_list::insert_back(int a)
{
    node *temp;
    temp = new node;
    std::cout << "Inserted at back : " << a << std::endl;
    temp->data = a;
    temp->next = NULL; // Элемент в конце не имеет следующего
    temp->prev = tail; // Предыдущий элемент - это старый tail
    
    // Если список пуст, новый элемент становится и первым, и последним
    if (tail == NULL)
    {
        head = temp;
        tail = temp;
    }
    else
    {
        // Иначе обновляем связь со старым последним элементом
        tail->next = temp;
        tail = temp;
    }
}

void doubly_linked_list::delete_front()
{
    if (head != NULL)
    {
        node *temp = head;
        head = head->next;
        std::cout << temp->data << " -- deleted from front" << std::endl;
        
        // Если список стал пустым, обнуляем и tail
        if (head == NULL)
        {
            tail = NULL;
        }
        else
        {
            // Иначе обнуляем связь с удаленным элементом
            head->prev = NULL;
        }
        
        delete temp;
    }
    else
    {
        std::cout << "List empty" << std::endl;
    }
}

void doubly_linked_list::delete_back()
{
    if (tail != NULL)
    {
        node *temp = tail;
        tail = tail->prev;
        std::cout << temp->data << " -- deleted from back" << std::endl;
        
        // Если список стал пустым, обнуляем и head
        if (tail == NULL)
        {
            head = NULL;
        }
        else
        {
            // Иначе обнуляем связь с удаленным элементом
            tail->next = NULL;
        }
        
        delete temp;
    }
    else
    {
        std::cout << "List empty" << std::endl;
    }
}

void doubly_linked_list::display_forward()
{
    std::cout << "List display (forward) : " << std::endl;
    node *temp = head;
    while (temp != NULL)
    {
        std::cout << temp->data << " <--> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

void doubly_linked_list::display_backward()
{
    std::cout << "List display (backward) : " << std::endl;
    node *temp = tail;
    std::cout << "NULL <--> ";
    while (temp != NULL)
    {
        std::cout << temp->data;
        if (temp->prev != NULL)
        {
            std::cout << " <--> ";
        }
        temp = temp->prev;
    }
    std::cout << std::endl;
}

doubly_linked_list::~doubly_linked_list()
{
    while (head != NULL)
    {
        node *temp = head;
        head = head->next;
        delete temp;
    }
    tail = NULL; // Для безопасности обнуляем и tail
}

int main()
{
    doubly_linked_list dll;
    std::cout << "Doubly Linked List example" << std::endl;

    dll.insert_front(1);
    dll.insert_back(2);
    dll.insert_front(3);
    dll.insert_back(4);
    dll.insert_front(5);

    dll.display_forward();
    dll.display_backward();

    dll.delete_front();
    dll.delete_back();

    dll.display_forward();
    dll.display_backward();

    return 0;
}


