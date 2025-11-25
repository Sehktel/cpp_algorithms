/**
 * Простой Ping-Pong TCP сервер для Linux
 * Упрощенная версия для демонстрации на доске
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    std::cout << "=== Простой Ping-Pong сервер ===" << std::endl;
    
    // 1. Создание сокета
    // socket(семейство_адресов, тип_сокета, протокол)
    int serverSocket = socket(AF_INET,        // IPv4 адреса
                              SOCK_STREAM,     // TCP (потоковый)
                              0);              // Автовыбор протокола
    if (serverSocket < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }
    std::cout << "Сокет создан" << std::endl;
    
    // 2. Настройка адреса сервера
    sockaddr_in serverAddr;                    // Структура адреса IPv4
    memset(&serverAddr, 0, sizeof(serverAddr)); // Обнуление структуры
    serverAddr.sin_family = AF_INET;           // Семейство IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;   // Все интерфейсы (0.0.0.0)
    serverAddr.sin_port = htons(PORT);         // Порт (host to network short)
    
    // 3. Привязка сокета к адресу
    // bind(сокет, адрес, размер_адреса)
    if (bind(serverSocket,                    // Серверный сокет
             (sockaddr*)&serverAddr,          // Адрес для привязки
             sizeof(serverAddr))              // Размер структуры адреса
             < 0) {
        std::cerr << "Ошибка bind" << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "Сокет привязан к порту " << PORT << std::endl;
    
    // 4. Перевод в режим прослушивания
    // listen(сокет, размер_очереди)
    if (listen(serverSocket,                  // Серверный сокет
               3)                             // Размер очереди ожидающих соединений
                < 0) {
        std::cerr << "Ошибка listen" << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "Сервер слушает порт " << PORT << "..." << std::endl;
    
    // 5. Принятие подключения
    // accept(сокет, адрес_клиента, размер_адреса)
    std::cout << "Ожидание подключения клиента..." << std::endl;
    sockaddr_in clientAddr;                   // Структура адреса клиента
    socklen_t clientAddrSize = sizeof(clientAddr); // Размер структуры
    int clientSocket = accept(serverSocket,    // Серверный сокет
                              (sockaddr*)&clientAddr, // Адрес клиента
                              &clientAddrSize); // Размер структуры (указатель!)
    
    if (clientSocket < 0) {
        std::cerr << "Ошибка accept" << std::endl;
        close(serverSocket);
        return 1;
    }
    
    // Преобразование IP адреса клиента в строку
    // inet_ntop(семейство, адрес, буфер, размер_буфера)
    char clientIP[INET_ADDRSTRLEN];           // Буфер для IP строки
    inet_ntop(AF_INET,                        // IPv4
              &clientAddr.sin_addr,          // Бинарный адрес
              clientIP,                       // Буфер для строки
              INET_ADDRSTRLEN);              // Размер буфера
    std::cout << "Клиент подключен: " << clientIP << std::endl;
    
    // 6. Обработка сообщений
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);        // Обнуление буфера
        
        // Получение сообщения
        // recv(сокет, буфер, размер_буфера, флаги)
        int bytesReceived = recv(clientSocket, // Клиентский сокет
                                 buffer,       // Буфер для данных
                                 BUFFER_SIZE - 1, // Размер буфера (-1 для '\0')
                                 0);           // Флаги (обычно 0)
        if (bytesReceived <= 0) {
            std::cout << "Клиент отключился" << std::endl;
            break;
        }
        
        std::string message(buffer);
        std::cout << "Получено: " << message << std::endl;
        
        // Проверка команды выхода
        if (message == "exit") {
            std::cout << "Получена команда выхода" << std::endl;
            break;
        }
        
        // Формирование ответа
        std::string response;
        if (message == "ping") {
            response = "pong";
        } else {
            response = "Неизвестная команда";
        }
        
        // Отправка ответа
        // send(сокет, данные, размер_данных, флаги)
        std::cout << "Отправка: " << response << std::endl;
        send(clientSocket,                    // Клиентский сокет
             response.c_str(),                // Данные (C-строка)
             response.length(),               // Размер данных
             0);                              // Флаги (обычно 0)
    }
    
    // 7. Закрытие соединений
    // close(файловый_дескриптор)
    close(clientSocket);                      // Закрытие клиентского сокета
    close(serverSocket);                      // Закрытие серверного сокета
    std::cout << "Сервер завершил работу" << std::endl;
    
    return 0;
}
