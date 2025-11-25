/**
 * Ping-Pong Client
 * Учебный пример TCP клиента на C++
 * 
 * Функционал:
 * - Подключается к серверу на localhost:8080
 * - Отправляет несколько сообщений "ping"
 * - Получает и выводит ответы "pong"
 * - Отправляет "exit" для завершения
 */

#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

// Подключение платформо-зависимых заголовков
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

const char* SERVER_IP = "127.0.0.1";  // localhost
const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;
const int PING_COUNT = 5;  // Количество ping сообщений

/**
 * Инициализация Windows Sockets (только для Windows)
 */
bool initializeWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "❌ Ошибка WSAStartup: " << result << std::endl;
        return false;
    }
    std::cout << "✅ Winsock инициализирован" << std::endl;
#endif
    return true;
}

/**
 * Очистка Windows Sockets (только для Windows)
 */
void cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
    std::cout << "🧹 Winsock очищен" << std::endl;
#endif
}

/**
 * Создание и подключение клиентского сокета
 */
SOCKET connectToServer() {
    // 1. Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "❌ Не удалось создать сокет" << std::endl;
        return INVALID_SOCKET;
    }
    std::cout << "✅ Сокет создан" << std::endl;

    // 2. Настройка адреса сервера
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Преобразование IP адреса из строки в бинарный формат
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "❌ Неверный IP адрес" << std::endl;
        closesocket(clientSocket);
        return INVALID_SOCKET;
    }

    // 3. Подключение к серверу
    std::cout << "🔄 Подключение к серверу " << SERVER_IP << ":" << SERVER_PORT << "..." << std::endl;
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Не удалось подключиться к серверу" << std::endl;
        std::cerr << "   Убедитесь, что сервер запущен!" << std::endl;
        closesocket(clientSocket);
        return INVALID_SOCKET;
    }
    std::cout << "✅ Успешно подключено к серверу!" << std::endl;

    return clientSocket;
}

/**
 * Отправка сообщения и получение ответа
 */
bool sendAndReceive(SOCKET socket, const std::string& message) {
    // Отправка сообщения
    std::cout << "📤 Отправка: \"" << message << "\"" << std::endl;
    int bytesSent = send(socket, message.c_str(), message.length(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "❌ Ошибка при отправке данных" << std::endl;
        return false;
    }

    // Если отправляем exit, не ждем ответа
    if (message == "exit") {
        return true;
    }

    // Получение ответа
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(socket, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytesReceived <= 0) {
        std::cerr << "❌ Ошибка при получении данных или сервер закрыл соединение" << std::endl;
        return false;
    }

    std::cout << "📩 Получено: \"" << buffer << "\"" << std::endl;
    return true;
}

/**
 * Интерактивный режим клиента
 */
void interactiveMode(SOCKET clientSocket) {
    std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
    std::cout << "║     ИНТЕРАКТИВНЫЙ РЕЖИМ              ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════╝" << std::endl;
    std::cout << "Введите 'ping' для отправки или 'exit' для выхода\n" << std::endl;

    std::string input;
    while (true) {
        std::cout << "Вы> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        if (!sendAndReceive(clientSocket, input)) {
            break;
        }

        if (input == "exit") {
            break;
        }

        std::cout << std::endl;
    }
}

int main() {
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     PING-PONG TCP КЛИЕНТ (C++)        ║" << std::endl;
    std::cout << "║     Учебный проект по сетям           ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    // Инициализация (Windows)
    if (!initializeWinsock()) {
        return 1;
    }

    // Подключение к серверу
    SOCKET clientSocket = connectToServer();
    if (clientSocket == INVALID_SOCKET) {
        cleanupWinsock();
        return 1;
    }

    std::cout << "\n─────────────────────────────────────────" << std::endl;
    std::cout << "  Режим работы:" << std::endl;
    std::cout << "  1 - Автоматический (отправить " << PING_COUNT << " ping)" << std::endl;
    std::cout << "  2 - Интерактивный" << std::endl;
    std::cout << "─────────────────────────────────────────" << std::endl;
    std::cout << "Выберите режим (1/2): ";

    std::string mode;
    std::getline(std::cin, mode);

    if (mode == "1") {
        // Автоматический режим
        std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
        std::cout << "║     АВТОМАТИЧЕСКИЙ РЕЖИМ             ║" << std::endl;
        std::cout << "╚═══════════════════════════════════════╝" << std::endl;
        std::cout << "Отправка " << PING_COUNT << " ping сообщений...\n" << std::endl;

        for (int i = 1; i <= PING_COUNT; i++) {
            std::cout << "--- Сообщение " << i << "/" << PING_COUNT << " ---" << std::endl;
            
            if (!sendAndReceive(clientSocket, "ping")) {
                break;
            }

            std::cout << std::endl;

            // Небольшая задержка между сообщениями
            if (i < PING_COUNT) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        // Отправка команды завершения
        std::cout << "--- Завершение ---" << std::endl;
        sendAndReceive(clientSocket, "exit");
    } else {
        // Интерактивный режим
        interactiveMode(clientSocket);
    }

    // Закрытие сокета
    closesocket(clientSocket);
    std::cout << "\n✅ Соединение закрыто" << std::endl;

    // Очистка (Windows)
    cleanupWinsock();

    std::cout << "👋 Клиент завершил работу" << std::endl;
    return 0;
}

