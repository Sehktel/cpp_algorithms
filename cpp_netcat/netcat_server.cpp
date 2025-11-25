// ⚠️ LEGACY: Этот файл мигрирован в 02_networking/08_tcp_client_server/netcat/netcat_server.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 02_networking/08_tcp_client_server/netcat/netcat_server.cpp

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

// Функция для парсинга аргументов командной строки
struct NetworkConfig {
    std::string output_file;
    int port = 8080;
    std::string network_interface = "127.0.0.1";
};

// Функция для разбора аргументов командной строки
NetworkConfig parse_arguments(int argc, char* argv[]) {
    NetworkConfig config;
    int opt;
    while ((opt = getopt(argc, argv, "o:p:n:")) != -1) {
        switch (opt) {
            case 'o':
                config.output_file = optarg;
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
    if (config.output_file.empty()) {
        throw std::runtime_error("Необходимо указать выходной файл (-o)");
    }

    return config;
}

// Функция для приема файла через сокет
void receive_file(const NetworkConfig& config) {
    // Создание сокета
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        throw std::runtime_error("Не удалось создать сокет");
    }

    // Настройка адреса сервера
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(config.port);
    inet_pton(AF_INET, config.network_interface.c_str(), &address.sin_addr);

    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(server_fd);
        throw std::runtime_error("Не удалось привязать сокет");
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 1) < 0) {
        close(server_fd);
        throw std::runtime_error("Ошибка прослушивания");
    }

    std::cout << "Ожидание подключения на " << config.network_interface 
              << ":" << config.port << std::endl;

    // Принятие соединения
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
    if (client_socket < 0) {
        close(server_fd);
        throw std::runtime_error("Ошибка принятия соединения");
    }

    // Открытие файла для записи
    std::ofstream output_file(config.output_file, std::ios::binary);
    if (!output_file) {
        close(server_fd);
        close(client_socket);
        throw std::runtime_error("Не удалось открыть выходной файл");
    }

    // Прием данных
    char buffer[4096];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        output_file.write(buffer, bytes_received);
    }

    // Закрытие соединений
    output_file.close();
    close(client_socket);
    close(server_fd);

    std::cout << "Файл успешно принят: " << config.output_file << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        auto config = parse_arguments(argc, argv);
        receive_file(config);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 