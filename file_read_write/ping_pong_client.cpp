/*
 * Простой клиент для тестирования пинг-понг сервера
 * 
 * Функциональность:
 * - Подключение к серверу по адресу и порту из settings.ini
 * - Получение текстового файла от сервера
 * - Игра в пинг-понг с сервером
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

// Класс для чтения настроек (можно переиспользовать тот же, что и в сервере)
class ConfigReader {
private:
    string host;
    int port;
    string filename;
    
public:
    ConfigReader() : port(0) {}
    
    bool loadConfig(const string& configFile = "settings.ini") {
        ifstream file(configFile);
        if (!file.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл конфигурации " << configFile << endl;
            return false;
        }
        
        string line;
        while (getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            size_t pos = line.find('=');
            if (pos == string::npos) {
                continue;
            }
            
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (key == "host") {
                host = value;
            } else if (key == "port") {
                port = stoi(value);
            } else if (key == "filename") {
                filename = value;
            }
        }
        
        file.close();
        
        if (host.empty() || port <= 0) {
            cerr << "Ошибка: Не все необходимые параметры найдены в конфигурации" << endl;
            return false;
        }
        
        return true;
    }
    
    const string& getHost() const { return host; }
    int getPort() const { return port; }
};

// Класс клиента для пинг-понг
class PingPongClient {
private:
    ConfigReader config;
    SOCKET clientSocket;
    
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
    
public:
    PingPongClient() : clientSocket(INVALID_SOCKET) {}
    
    ~PingPongClient() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
#ifdef _WIN32
        WSACleanup();
#endif
    }
    
    // Подключение к серверу
    bool connectToServer() {
        if (!config.loadConfig()) {
            return false;
        }
        
        if (!initializeSockets()) {
            return false;
        }
        
        // Создаём сокет
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Ошибка создания сокета" << endl;
            return false;
        }
        
        // Настраиваем адрес сервера
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(config.getPort());
        
        // Преобразуем строку IP в нужный формат
        if (config.getHost() == "localhost") {
            serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        } else {
            inet_pton(AF_INET, config.getHost().c_str(), &serverAddr.sin_addr);
        }
        
        // Подключаемся к серверу
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Ошибка подключения к серверу " << config.getHost() << ":" << config.getPort() << endl;
            return false;
        }
        
        cout << "Успешно подключились к серверу " << config.getHost() << ":" << config.getPort() << endl;
        return true;
    }
    
    // Получение файла от сервера
    bool receiveFile() {
        // Получаем размер файла
        size_t fileSize;
        int bytesReceived = recv(clientSocket, (char*)&fileSize, sizeof(fileSize), 0);
        if (bytesReceived != sizeof(fileSize)) {
            cerr << "Ошибка получения размера файла" << endl;
            return false;
        }
        
        // Выделяем буфер для содержимого файла
        vector<char> buffer(fileSize);
        
        // Получаем содержимое файла
        size_t totalReceived = 0;
        while (totalReceived < fileSize) {
            bytesReceived = recv(clientSocket, buffer.data() + totalReceived, 
                               fileSize - totalReceived, 0);
            if (bytesReceived <= 0) {
                cerr << "Ошибка получения содержимого файла" << endl;
                return false;
            }
            totalReceived += bytesReceived;
        }
        
        // Выводим полученный файл
        string fileContent(buffer.data(), fileSize);
        cout << "\n=== Полученный файл от сервера ===" << endl;
        cout << fileContent << endl;
        cout << "=== Конец файла ===" << endl;
        
        return true;
    }
    
    // Игра в пинг-понг
    void playPingPong() {
        const int MAX_BUFFER_SIZE = 1024;
        char buffer[MAX_BUFFER_SIZE];
        int messageCount = 0;
        
        cout << "\nНачинаем игру в пинг-понг!" << endl;
        
        while (true) {
            // Отправляем сообщение серверу
            messageCount++;
            string message = "PING #" + to_string(messageCount) + " от клиента";
            
            if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
                cerr << "Ошибка отправки сообщения серверу" << endl;
                break;
            }
            
            cout << "Отправлено серверу: " << message << endl;
            
            // Получаем ответ от сервера
            memset(buffer, 0, MAX_BUFFER_SIZE);
            int bytesReceived = recv(clientSocket, buffer, MAX_BUFFER_SIZE - 1, 0);
            
            if (bytesReceived <= 0) {
                cout << "Сервер отключился или произошла ошибка" << endl;
                break;
            }
            
            string receivedMessage(buffer);
            cout << "Получено от сервера: " << receivedMessage << endl;
            
            // Завершаем игру после нескольких обменов
            if (messageCount >= 5) {
                string exitMessage = "EXIT";
                send(clientSocket, exitMessage.c_str(), exitMessage.length(), 0);
                cout << "Завершение игры в пинг-понг" << endl;
                break;
            }
            
            // Небольшая пауза между сообщениями
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
};

int main() {
    cout << "=== Пинг-понг клиент ===" << endl;
    
    PingPongClient client;
    
    // Подключаемся к серверу
    if (!client.connectToServer()) {
        cerr << "Не удалось подключиться к серверу" << endl;
        return 1;
    }
    
    try {
        // Получаем файл от сервера
        if (!client.receiveFile()) {
            cerr << "Не удалось получить файл от сервера" << endl;
            return 1;
        }
        
        // Играем в пинг-понг
        client.playPingPong();
        
    } catch (const exception& e) {
        cerr << "Исключение: " << e.what() << endl;
    }
    
    cout << "\nКлиент завершил работу" << endl;
    return 0;
}
