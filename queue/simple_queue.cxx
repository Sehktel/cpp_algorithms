#include <iostream>

// Определяем структуру поля
struct node
{
    int data;
    node *next;
};

// Определяем класс очередь
class queue
{
    // Указатели на начало и конец очереди
    node *front; // Указатель на первый элемент (откуда забираем)
    node *rear;  // Указатель на последний элемент (куда добавляем)

    // публичные методы
    public:
        // Конструктор объекта очереди
        queue() 
        {
            front = NULL; // При создании очереди начало указывает в никуда
            rear = NULL;  // При создании очереди конец тоже указывает в никуда
        }
        void enqueue(int a); // Добавить элемент в конец очереди
        void dequeue();      // Забрать элемент из начала очереди (у нас void который ничего не возвращает)
        void display();      // Отобразить содержимое очереди
        ~queue();            // Деструктор объекта
};

void queue::enqueue(int a)
{
    node *temp;
    temp = new node;
    std::cout << "Enqueued data : " << a << std::endl;
    temp->data = a;
    temp->next = NULL; // Новый элемент всегда становится последним
    
    // Если очередь пуста, новый элемент становится и первым, и последним
    if (rear == NULL)
    {
        front = temp;
        rear = temp;
    }
    else
    {
        // Иначе добавляем в конец и обновляем указатель rear
        rear->next = temp;
        rear = temp;
    }
}

void queue::dequeue()
{
    if (front != NULL)
    {
        node *temp = front;
        front = front->next;
        std::cout << temp->data << " -- deleted" << std::endl;
        
        // Если очередь стала пустой, обнуляем и rear
        if (front == NULL)
        {
            rear = NULL;
        }
        
        delete temp;
    }
    else
    {
        std::cout << "Queue empty" << std::endl;
    }
}

void queue::display()
{
    std::cout << "Queue display " << std::endl;
    node *temp = front;
    while (temp != NULL)
    {
        std::cout << temp->data << " --> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

queue::~queue()
{
    while (front != NULL)
    {
        node *temp = front;
        front = front->next;
        delete temp;
    }
    rear = NULL; // Для безопасности обнуляем и rear
}

int main()
{
    queue q;
    std::cout << "Queue example" << std::endl;

    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);

    q.dequeue();
    q.dequeue();

    q.display();

    return 0;
}


