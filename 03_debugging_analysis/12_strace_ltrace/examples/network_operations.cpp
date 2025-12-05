/*
 * Пример программы для изучения strace с сетевыми операциями
 * 
 * Эта программа демонстрирует сетевые системные вызовы:
 * - socket()
 * - connect()
 * - send() / recv()
 * - bind() / listen() / accept()
 */

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Простая функция для создания TCP сокета
int create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }
    return sock;
}

// Функция для подключения к серверу
bool connect_to_server(int sock, const char* host, int port) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        std::cerr << "Ошибка преобразования адреса" << std::endl;
        return false;
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка подключения" << std::endl;
        return false;
    }
    
    return true;
}

// Функция для отправки данных
bool send_data(int sock, const char* data) {
    size_t len = strlen(data);
    ssize_t sent = send(sock, data, len, 0);
    if (sent < 0) {
        std::cerr << "Ошибка отправки данных" << std::endl;
        return false;
    }
    std::cout << "Отправлено " << sent << " байт" << std::endl;
    return true;
}

// Функция для получения данных
bool receive_data(int sock, char* buffer, size_t buffer_size) {
    ssize_t received = recv(sock, buffer, buffer_size - 1, 0);
    if (received < 0) {
        std::cerr << "Ошибка получения данных" << std::endl;
        return false;
    }
    buffer[received] = '\0';
    std::cout << "Получено " << received << " байт: " << buffer << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Использование: " << argv[0] << " <хост> <порт> <сообщение>" << std::endl;
        std::cout << "Пример: " << argv[0] << " 127.0.0.1 8080 \"Hello, Server!\"" << std::endl;
        return 1;
    }
    
    const char* host = argv[1];
    int port = std::stoi(argv[2]);
    const char* message = argv[3];
    
    std::cout << "=== Сетевая программа для strace ===" << std::endl;
    std::cout << "Подключение к " << host << ":" << port << std::endl;
    
    // Создать сокет
    int sock = create_socket();
    if (sock < 0) {
        return 1;
    }
    
    // Подключиться к серверу
    if (!connect_to_server(sock, host, port)) {
        close(sock);
        return 1;
    }
    
    std::cout << "Подключение установлено" << std::endl;
    
    // Отправить данные
    if (!send_data(sock, message)) {
        close(sock);
        return 1;
    }
    
    // Получить ответ (если сервер отвечает)
    char buffer[1024];
    receive_data(sock, buffer, sizeof(buffer));
    
    // Закрыть сокет
    close(sock);
    std::cout << "Соединение закрыто" << std::endl;
    
    return 0;
}


