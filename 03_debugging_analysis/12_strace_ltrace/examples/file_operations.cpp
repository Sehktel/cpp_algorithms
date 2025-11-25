/*
 * Пример программы для изучения strace
 * 
 * Эта программа демонстрирует различные системные вызовы:
 * - Открытие и чтение файлов
 * - Запись в файлы
 * - Работа с директориями
 * - Системная информация
 */

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>

// Функция для чтения файла
void read_file(const char* filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
    }
}

// Функция для записи в файл
void write_file(const char* filename, const char* content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "Не удалось создать файл: " << filename << std::endl;
    }
}

// Функция для получения информации о файле
void file_info(const char* filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        std::cout << "Размер файла: " << file_stat.st_size << " байт" << std::endl;
        std::cout << "Права доступа: " << std::oct << file_stat.st_mode << std::endl;
    } else {
        std::cerr << "Не удалось получить информацию о файле" << std::endl;
    }
}

// Функция для чтения директории
void list_directory(const char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::cout << entry->d_name << std::endl;
        }
        closedir(dir);
    } else {
        std::cerr << "Не удалось открыть директорию: " << dirname << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <команда> [аргументы]" << std::endl;
        std::cout << "Команды:" << std::endl;
        std::cout << "  read <файл>     - прочитать файл" << std::endl;
        std::cout << "  write <файл>    - записать в файл" << std::endl;
        std::cout << "  info <файл>     - информация о файле" << std::endl;
        std::cout << "  list <директория> - список файлов в директории" << std::endl;
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "read" && argc > 2) {
        read_file(argv[2]);
    } else if (command == "write" && argc > 3) {
        write_file(argv[2], argv[3]);
    } else if (command == "info" && argc > 2) {
        file_info(argv[2]);
    } else if (command == "list" && argc > 2) {
        list_directory(argv[2]);
    } else {
        std::cerr << "Неизвестная команда или недостаточно аргументов" << std::endl;
        return 1;
    }
    
    return 0;
}

