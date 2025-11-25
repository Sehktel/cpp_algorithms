/**
 * Простой Ping-Pong TCP клиент для Linux
 * Упрощенная версия для демонстрации на доске
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";  // localhost
const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    std::cout << "=== Простой Ping-Pong клиент ===" << std::endl;
    
    // 1. Создание сокета
    // socket(семейство_адресов, тип_сокета, протокол)
    int clientSocket = socket(AF_INET,        // IPv4 адреса
                              SOCK_STREAM,     // TCP (потоковый)
                              0);              // Автовыбор протокола
    if (clientSocket < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }
    std::cout << "Сокет создан" << std::endl;
    
    // 2. Настройка адреса сервера
    sockaddr_in serverAddr;                    // Структура адреса IPv4
    memset(&serverAddr, 0, sizeof(serverAddr)); // Обнуление структуры
    serverAddr.sin_family = AF_INET;           // Семейство IPv4
    serverAddr.sin_port = htons(SERVER_PORT);  // Порт (host to network short)
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr); // IP в бинарный формат
    
    // 3. Подключение к серверу
    // connect(сокет, адрес_сервера, размер_адреса)
    std::cout << "Подключение к серверу..." << std::endl;
    if (connect(clientSocket,                    // Наш сокет
                (sockaddr*)&serverAddr,          // Адрес сервера (приведение типа)
                sizeof(serverAddr))              // Размер структуры адреса
                < 0) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        close(clientSocket);
        return 1;
    }
    std::cout << "Подключено к серверу!" << std::endl;
    
    // 4. Отправка ping сообщений
    for (int i = 1; i <= 3; i++) {
        std::string message = "ping";
        
        // Отправка
        // send(сокет, данные, размер_данных, флаги)
        std::cout << "Отправка: " << message << std::endl;
        send(clientSocket,                    // Сокет
             message.c_str(),                 // Данные (C-строка)
             message.length(),                // Размер данных
             0);                              // Флаги (обычно 0)
        
        // Получение ответа
        // recv(сокет, буфер, размер_буфера, флаги)
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);       // Обнуление буфера
        int bytesReceived = recv(clientSocket, // Сокет
                                 buffer,       // Буфер для данных
                                 BUFFER_SIZE - 1, // Размер буфера (-1 для '\0')
                                 0);          // Флаги (обычно 0)
        
        if (bytesReceived > 0) {
            std::cout << "Получено: " << buffer << std::endl;
        }
        
        std::cout << "---" << std::endl;
    }
    
    // 5. Завершение
    // send(сокет, данные, размер_данных, флаги)
    std::cout << "Отправка команды завершения..." << std::endl;
    send(clientSocket,                    // Сокет
         "exit",                          // Команда завершения
         4,                               // Размер строки "exit"
         0);                              // Флаги
    
    // 6. Закрытие сокета
    // close(файловый_дескриптор)
    close(clientSocket);                  // Закрытие сокета
    std::cout << "Соединение закрыто" << std::endl;
    
    return 0;
}
