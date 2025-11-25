/*
 * Пинг-понг сервер с использованием объектов
 * 
 * Функциональность:
 * - Чтение настроек из файла settings.ini (хост, порт)
 * - Установка TCP соединения
 * - Игра в пинг-понг
 * - Передача текстового файла после установки соединения
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>

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
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

using namespace std;

// Класс для чтения настроек из INI файла
class ConfigReader {
private:
    string host;
    int port;
    string filename;
    
public:
    ConfigReader() : port(0) {}
    
    // Чтение настроек из файла settings.ini
    bool loadConfig(const string& configFile = "settings.ini") {
        ifstream file(configFile);
        if (!file.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл конфигурации " << configFile << endl;
            return false;
        }
        
        string line;
        while (getline(file, line)) {
            // Удаляем пробелы в начале и конце строки
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            // Пропускаем пустые строки и комментарии
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            // Ищем разделитель '='
            size_t pos = line.find('=');
            if (pos == string::npos) {
                continue;
            }
            
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            // Убираем пробелы вокруг ключа и значения
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Парсим настройки
            if (key == "host") {
                host = value;
            } else if (key == "port") {
                port = stoi(value);
            } else if (key == "filename") {
                filename = value;
            }
        }
        
        file.close();
        
        // Проверяем, что все необходимые параметры считаны
        if (host.empty() || port <= 0 || filename.empty()) {
            cerr << "Ошибка: Не все необходимые параметры найдены в конфигурации" << endl;
            cerr << "Требуются: host, port, filename" << endl;
            return false;
        }
        
        return true;
    }
    
    // Геттеры для доступа к настройкам
    const string& getHost() const { return host; }
    int getPort() const { return port; }
    const string& getFilename() const { return filename; }
};

// Класс пинг-понг сервера
class PingPongServer {
private:
    ConfigReader config;
    SOCKET serverSocket;
    SOCKET clientSocket;
    bool isRunning;
    
    // Инициализация сокетов для Windows
    bool initializeSockets() {
#ifdef _WIN32
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            cerr << "Ошибка инициализации Winsock: " << result << endl;
            return false;
        }
#endif
        return true;
    }
    
    // Очистка сокетов
    void cleanupSockets() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
#ifdef _WIN32
        WSACleanup();
#endif
    }
    
public:
    PingPongServer() : serverSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET), isRunning(false) {}
    
    ~PingPongServer() {
        stop();
    }
    
    // Запуск сервера
    bool start() {
        if (!config.loadConfig()) {
            return false;
        }
        
        if (!initializeSockets()) {
            return false;
        }
        
        // Создаём сокет
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            cerr << "Ошибка создания сокета" << endl;
            cleanupSockets();
            return false;
        }
        
        // Настраиваем адрес сервера
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Принимаем подключения на любой интерфейс
        serverAddr.sin_port = htons(config.getPort());
        
        // Привязываем сокет к адресу
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Ошибка привязки сокета к порту " << config.getPort() << endl;
            cleanupSockets();
            return false;
        }
        
        // Начинаем прослушивание
        if (listen(serverSocket, 1) == SOCKET_ERROR) {
            cerr << "Ошибка начала прослушивания" << endl;
            cleanupSockets();
            return false;
        }
        
        cout << "Сервер запущен на " << config.getHost() << ":" << config.getPort() << endl;
        cout << "Ожидание подключения клиента..." << endl;
        
        isRunning = true;
        return true;
    }
    
    // Ожидание подключения клиента
    bool waitForClient() {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Ошибка принятия подключения" << endl;
            return false;
        }
        
        cout << "Клиент подключен!" << endl;
        return true;
    }
    
    // Передача текстового файла клиенту
    bool sendFile() {
        ifstream file(config.getFilename());
        if (!file.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл " << config.getFilename() << endl;
            return false;
        }
        
        // Читаем весь файл в строку
        stringstream buffer;
        buffer << file.rdbuf();
        string fileContent = buffer.str();
        file.close();
        
        // Отправляем размер файла
        size_t fileSize = fileContent.size();
        if (send(clientSocket, (char*)&fileSize, sizeof(fileSize), 0) == SOCKET_ERROR) {
            cerr << "Ошибка отправки размера файла" << endl;
            return false;
        }
        
        // Отправляем содержимое файла
        if (send(clientSocket, fileContent.c_str(), fileSize, 0) == SOCKET_ERROR) {
            cerr << "Ошибка отправки содержимого файла" << endl;
            return false;
        }
        
        cout << "Файл " << config.getFilename() << " отправлен клиенту (размер: " << fileSize << " байт)" << endl;
        return true;
    }
    
    // Игра в пинг-понг
    void playPingPong() {
        const int MAX_BUFFER_SIZE = 1024;
        char buffer[MAX_BUFFER_SIZE];
        int messageCount = 0;
        
        cout << "Начинаем игру в пинг-понг!" << endl;
        
        while (isRunning) {
            // Получаем сообщение от клиента
            memset(buffer, 0, MAX_BUFFER_SIZE);
            int bytesReceived = recv(clientSocket, buffer, MAX_BUFFER_SIZE - 1, 0);
            
            if (bytesReceived <= 0) {
                cout << "Клиент отключился или произошла ошибка" << endl;
                break;
            }
            
            string receivedMessage(buffer);
            cout << "Получено от клиента: " << receivedMessage << endl;
            messageCount++;
            
            // Проверяем, не пора ли закончить игру
            if (receivedMessage.find("EXIT") != string::npos || messageCount >= 10) {
                cout << "Завершение игры в пинг-понг" << endl;
                break;
            }
            
            // Формируем ответ
            string response = "PONG #" + to_string(messageCount) + " от сервера";
            
            // Отправляем ответ клиенту
            if (send(clientSocket, response.c_str(), response.length(), 0) == SOCKET_ERROR) {
                cerr << "Ошибка отправки ответа клиенту" << endl;
                break;
            }
            
            cout << "Отправлено клиенту: " << response << endl;
            
            // Небольшая пауза для демонстрации
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
    
    // Остановка сервера
    void stop() {
        isRunning = false;
        cleanupSockets();
        cout << "Сервер остановлен" << endl;
    }
};

int main() {
    cout << "=== Пинг-понг сервер ===" << endl;
    
    PingPongServer server;
    
    // Запускаем сервер
    if (!server.start()) {
        cerr << "Не удалось запустить сервер" << endl;
        return 1;
    }
    
    try {
        // Ожидаем подключения клиента
        if (!server.waitForClient()) {
            cerr << "Не удалось принять подключение клиента" << endl;
            return 1;
        }
        
        // Сначала отправляем файл
        if (!server.sendFile()) {
            cerr << "Не удалось отправить файл" << endl;
            return 1;
        }
        
        // Затем начинаем игру в пинг-понг
        server.playPingPong();
        
    } catch (const exception& e) {
        cerr << "Исключение: " << e.what() << endl;
    }
    
    server.stop();
    return 0;
}

/*
 * АРХИТЕКТУРНЫЕ РЕШЕНИЯ И КОММЕНТАРИИ:
 * 
 * 1. ОБЪЕКТНО-ОРИЕНТИРОВАННЫЙ ПОДХОД:
 *    - ConfigReader: инкапсулирует чтение настроек из INI файла
 *    - PingPongServer: основной класс сервера с методами для управления
 * 
 * 2. КРОССПЛАТФОРМЕННОСТЬ:
 *    - Поддержка Windows (Winsock) и Unix/Linux сокетов
 *    - Автоматическое определение платформы через #ifdef
 * 
 * 3. ОБРАБОТКА ОШИБОК:
 *    - Проверка всех критических операций с сокетами
 *    - Корректное освобождение ресурсов в деструкторе
 * 
 * 4. АЛЬТЕРНАТИВНЫЕ РЕШЕНИЯ:
 *    - Можно было бы использовать boost::asio для более современного подхода
 *    - Для продакшена стоит добавить пул потоков для множественных клиентов
 *    - Парсинг INI можно заменить на библиотеку типа inih
 * 
 * 5. ПРОИЗВОДИТЕЛЬНОСТЬ:
 *    - Текущая реализация синхронная, блокирует поток
 *    - Для высоконагруженных систем нужна асинхронная архитектура
 */
