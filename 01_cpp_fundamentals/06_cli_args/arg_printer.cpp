#include <iostream>
#include <string>

// Функция для демонстрации обработки аргументов командной строки
int main(int argc, char* argv[]) {
    // Вывод общего количества аргументов
    std::cout << "Количество аргументов: " << argc << std::endl;

    // Классический цикл for для итерации по аргументам
    std::cout << "Список аргументов:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        // Вывод индекса и значения каждого аргумента
        std::cout << "Аргумент [" << i << "]: " << argv[i] << std::endl;
    }

    // Демонстрация обработки числовых аргументов
    if (argc > 1) {
        std::cout << "\nОбработка числовых аргументов:" << std::endl;
        int sum = 0;
        for (int i = 1; i < argc; ++i) {
            try {
                // Преобразование строкового аргумента в целое число
                int value = std::stoi(argv[i]);
                sum += value;
                std::cout << "Преобразовано: " << value << std::endl;
            } catch (const std::invalid_argument& e) {
                // Обработка ошибки преобразования
                std::cerr << "Невозможно преобразовать аргумент: " << argv[i] << std::endl;
            } catch (const std::out_of_range& e) {
                // Обработка переполнения
                std::cerr << "Значение вне допустимого диапазона: " << argv[i] << std::endl;
            }
        }
        std::cout << "Сумма числовых аргументов: " << sum << std::endl;
    }

    return 0;
} 