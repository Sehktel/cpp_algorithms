/*
 * Простая программа для отладки с GDB
 * 
 * Эта программа содержит несколько типичных ошибок и паттернов,
 * которые можно изучить с помощью GDB:
 * - Работа с указателями
 * - Массивы и границы
 * - Рекурсия
 * - Утечки памяти
 */

#include <iostream>
#include <cstring>
#include <cstdlib>

// Функция с потенциальной ошибкой переполнения буфера
void process_string(const char* input) {
    char buffer[16];
    // ⚠️ Потенциальная проблема: нет проверки длины
    strcpy(buffer, input);
    std::cout << "Обработанная строка: " << buffer << std::endl;
}

// Функция с проблемой указателя
int calculate_sum(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i <= size; i++) {  // ⚠️ Ошибка: должно быть <, а не <=
        sum += arr[i];
    }
    return sum;
}

// Рекурсивная функция для изучения стека вызовов
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Функция с утечкой памяти
void memory_leak_example() {
    int* data = new int[100];
    // ⚠️ Проблема: память не освобождается
    // delete[] data;  // Закомментировано для демонстрации
}

int main(int argc, char* argv[]) {
    std::cout << "=== Программа для отладки с GDB ===" << std::endl;
    
    // Пример 1: Работа со строками
    if (argc > 1) {
        process_string(argv[1]);
    } else {
        process_string("Короткая строка");
    }
    
    // Пример 2: Работа с массивом
    int numbers[] = {1, 2, 3, 4, 5};
    int sum = calculate_sum(numbers, 5);
    std::cout << "Сумма: " << sum << std::endl;
    
    // Пример 3: Рекурсия
    int fact = factorial(5);
    std::cout << "Факториал 5: " << fact << std::endl;
    
    // Пример 4: Утечка памяти
    memory_leak_example();
    
    return 0;
}

