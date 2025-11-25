// ⚠️ LEGACY: Этот файл мигрирован в 02_networking/08_tcp_client_server/server.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 02_networking/08_tcp_client_server/server.cpp

/**
 * Ping-Pong Server
 * Учебный пример TCP сервера на C++
 * 
 * Функционал:
 * - Прослушивает входящие подключения на порту 8080
 * - Принимает сообщения от клиента
 * - Отвечает "pong" на каждое "ping"
 * - Завершает работу при получении "exit"
 */

#include <iostream>
#include <string>
#include <cstring>

// Подключение платформо-зависимых заголовков
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
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

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

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
 * Создание и настройка серверного сокета
 */
SOCKET createServerSocket() {
    // 1. Создание сокета
    // AF_INET - IPv4
    // SOCK_STREAM - TCP (потоковый протокол)
    // 0 - автоматический выбор протокола
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "❌ Не удалось создать сокет" << std::endl;
        return INVALID_SOCKET;
    }
    std::cout << "✅ Сокет создан" << std::endl;

    // 2. Настройка адреса сервера
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;           // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;   // Прослушивать все интерфейсы
    serverAddr.sin_port = htons(PORT);         // Порт (htons - host to network short)

    // 3. Привязка сокета к адресу
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Ошибка bind()" << std::endl;
        closesocket(serverSocket);
        return INVALID_SOCKET;
    }
    std::cout << "✅ Сокет привязан к порту " << PORT << std::endl;

    // 4. Перевод сокета в режим прослушивания
    // Параметр 3 - размер очереди ожидающих соединений
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "❌ Ошибка listen()" << std::endl;
        closesocket(serverSocket);
        return INVALID_SOCKET;
    }
    std::cout << "✅ Сервер слушает порт " << PORT << "..." << std::endl;

    return serverSocket;
}

/**
 * Обработка клиентского соединения
 */
void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    
    std::cout << "\n📨 Клиент подключен! Ожидание сообщений..." << std::endl;
    std::cout << "─────────────────────────────────────────" << std::endl;

    while (true) {
        // Очистка буфера
        memset(buffer, 0, BUFFER_SIZE);

        // Получение данных от клиента
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytesReceived <= 0) {
            if (bytesReceived == 0) {
                std::cout << "🔌 Клиент отключился" << std::endl;
            } else {
                std::cerr << "❌ Ошибка при получении данных" << std::endl;
            }
            break;
        }

        std::string message(buffer);
        std::cout << "📩 Получено: \"" << message << "\"" << std::endl;

        // Проверка на команду выхода
        if (message == "exit") {
            std::cout << "👋 Получена команда выхода" << std::endl;
            break;
        }

        // Формирование ответа
        std::string response;
        if (message == "ping") {
            response = "pong";
            std::cout << "📤 Отправка: \"" << response << "\"" << std::endl;
        } else {
            response = "Неизвестная команда. Отправьте 'ping' или 'exit'";
            std::cout << "⚠️  Неизвестная команда от клиента" << std::endl;
        }

        // Отправка ответа клиенту
        int bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "❌ Ошибка при отправке данных" << std::endl;
            break;
        }
    }

    std::cout << "─────────────────────────────────────────" << std::endl;
    std::cout << "✅ Соединение с клиентом закрыто\n" << std::endl;
}

int main() {
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     PING-PONG TCP СЕРВЕР (C++)        ║" << std::endl;
    std::cout << "║     Учебный проект по сетям           ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    // Инициализация (Windows)
    if (!initializeWinsock()) {
        return 1;
    }

    // Создание серверного сокета
    SOCKET serverSocket = createServerSocket();
    if (serverSocket == INVALID_SOCKET) {
        cleanupWinsock();
        return 1;
    }

    // Основной цикл сервера
    while (true) {
        std::cout << "⏳ Ожидание подключения клиента..." << std::endl;

        // Принятие входящего соединения
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "❌ Ошибка accept()" << std::endl;
            continue;
        }

        // Вывод информации о клиенте
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "✅ Подключение от: " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

        // Обработка клиента
        handleClient(clientSocket);

        // Закрытие соединения с клиентом
        closesocket(clientSocket);

        // Вопрос о продолжении работы
        std::cout << "❓ Продолжить работу сервера? (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "n" || answer == "N") {
            std::cout << "🛑 Остановка сервера..." << std::endl;
            break;
        }
    }

    // Закрытие серверного сокета
    closesocket(serverSocket);
    std::cout << "✅ Серверный сокет закрыт" << std::endl;

    // Очистка (Windows)
    cleanupWinsock();

    std::cout << "\n👋 Сервер завершил работу" << std::endl;
    return 0;
}

