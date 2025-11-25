#include <iostream>

// Определяем структуру поля
struct node
{
    int data;
    node *next;
};

// Определяем класс стек
class stack
{
    // Указатель на вершину стека
    node *top;

    // публичные методы
    public:
        // Конструктор объекат стека
        stack() 
        {
            top = NULL; // При создании стека его вершина указывает в никуда 
        }
        void push(int a); // Положить в стек "что-то"
        void pop();       // Забрать из вершины (у нас void который ничего не возвращает)
        void display();   // Отобразить содержимое стека
        ~stack();         // Деструктор объекта
};

void stack::push(int a)
{
    node *temp;
    temp = new node;
    std::cout << "Pushed data : " << a << std::endl;
    temp->data = a;
    temp->next = top;
    top = temp;
}

void stack::pop()
{
    if (top != NULL)
    {
        node *temp = top;
        top = top->next;
        std::cout << temp->data << " -- deleted" << std::endl;
        delete temp;
    }
    else
    {
        std::cout << "Stack empty";
    }
}

void stack::display()
{
    std::cout << "Stack display " << std::endl;
    node *temp = top;
    while (temp != NULL)
    {
        std::cout << temp->data << " --> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

stack::~stack()
{
    while (top != NULL)
    {
        node *temp = top;
        top = top->next;
        delete temp;
    }
}

int main()
{
    stack st;
    std::cout << "Stack example" << std::endl;

    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);

    st.pop();
    st.pop();

    st.display();

    return 0;
}