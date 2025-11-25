// ⚠️ LEGACY: Этот файл мигрирован в 02_networking/08_tcp_client_server/netcat/netcat_client.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 02_networking/08_tcp_client_server/netcat/netcat_client.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <filesystem>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Структура для хранения конфигурации сети
struct NetworkConfig {
    std::string input_file;
    int port = 8080;
    std::string network_interface = "127.0.0.1";
};

// Функция для разбора аргументов командной строки
NetworkConfig parse_arguments(int argc, char* argv[]) {
    NetworkConfig config;
    int opt;
    while ((opt = getopt(argc, argv, "i:p:n:")) != -1) {
        switch (opt) {
            case 'i':
                config.input_file = optarg;
                break;
            case 'p':
                config.port = std::stoi(optarg);
                break;
            case 'n':
                config.network_interface = optarg;
                break;
            default:
                throw std::runtime_error("Неверные аргументы командной строки");
        }
    }

    // Проверка обязательных параметров
    if (config.input_file.empty()) {
        throw std::runtime_error("Необходимо указать входной файл (-i)");
    }

    return config;
}

// Функция для отправки файла через сокет
void send_file(const NetworkConfig& config) {
    // Открытие входного файла
    std::ifstream input_file(config.input_file, std::ios::binary);
    if (!input_file) {
        throw std::runtime_error("Не удалось открыть входной файл");
    }

    // Создание сокета
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        throw std::runtime_error("Не удалось создать сокет");
    }

    // Настройка адреса сервера
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(config.port);
    inet_pton(AF_INET, config.network_interface.c_str(), &server_address.sin_addr);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        close(client_socket);
        throw std::runtime_error("Не удалось подключиться к серверу");
    }

    std::cout << "Подключение к " << config.network_interface 
              << ":" << config.port << std::endl;

    // Отправка данных
    char buffer[4096];
    while (!input_file.eof()) {
        input_file.read(buffer, sizeof(buffer));
        std::streamsize bytes_read = input_file.gcount();
        
        if (bytes_read > 0) {
            ssize_t bytes_sent = send(client_socket, buffer, bytes_read, 0);
            if (bytes_sent < 0) {
                throw std::runtime_error("Ошибка при отправке данных");
            }
        }
    }

    // Закрытие соединений
    input_file.close();
    close(client_socket);

    std::cout << "Файл успешно отправлен: " << config.input_file << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        auto config = parse_arguments(argc, argv);
        send_file(config);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 