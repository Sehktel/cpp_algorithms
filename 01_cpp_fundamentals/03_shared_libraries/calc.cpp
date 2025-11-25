#include <iostream>
#include <string>
#include <stdexcept>
#include "my_math.h"

// Функция для выполнения математической операции
int perform_operation(int a, int b, const std::string& op) {
    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "*") return multiply(a, b);
    if (op == "/") return divide(a, b);
    
    // Обработка неизвестной операции
    throw std::runtime_error("Неподдерживаемая операция: " + op);
}

int main(int argc, char* argv[]) {
    // Проверка корректности количества аргументов
    if (argc != 4) {
        std::cerr << "Использование: " << argv[0] << " <число1> <операция> <число2>" << std::endl;
        return 1;
    }

    try {
        // Преобразование аргументов
        int a = std::stoi(argv[1]);
        std::string op = argv[2];
        int b = std::stoi(argv[3]);

        // Выполнение операции
        int result = perform_operation(a, b, op);
        
        // Вывод результата
        std::cout << a << " " << op << " " << b << " = " << result << std::endl;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка: Некорректный аргумент" << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: Число вне допустимого диапазона" << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 