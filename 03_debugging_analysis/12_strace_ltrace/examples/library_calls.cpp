/*
 * Пример программы для изучения ltrace
 * 
 * Эта программа демонстрирует вызовы библиотечных функций:
 * - Стандартная библиотека C (libc)
 * - Математические функции
 * - Строковые функции
 * - Функции времени
 */

#include <iostream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdlib>

// Функция, использующая математические функции
void math_operations() {
    double x = 3.14159;
    double y = 2.71828;
    
    std::cout << "sin(" << x << ") = " << sin(x) << std::endl;
    std::cout << "cos(" << x << ") = " << cos(x) << std::endl;
    std::cout << "sqrt(" << y << ") = " << sqrt(y) << std::endl;
    std::cout << "pow(" << x << ", 2) = " << pow(x, 2) << std::endl;
}

// Функция, использующая строковые функции
void string_operations() {
    char str1[100] = "Hello, ";
    char str2[] = "World!";
    
    strcat(str1, str2);
    std::cout << "Результат strcat: " << str1 << std::endl;
    
    size_t len = strlen(str1);
    std::cout << "Длина строки: " << len << std::endl;
    
    char* found = strstr(str1, "World");
    if (found) {
        std::cout << "Найдено 'World' в позиции: " << (found - str1) << std::endl;
    }
}

// Функция, использующая функции времени
void time_operations() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    std::cout << "Текущее время: " << buffer << std::endl;
}

// Функция, использующая функции памяти
void memory_operations() {
    int* arr1 = (int*)malloc(10 * sizeof(int));
    int* arr2 = (int*)calloc(10, sizeof(int));
    
    for (int i = 0; i < 10; i++) {
        arr1[i] = i * 2;
    }
    
    memcpy(arr2, arr1, 10 * sizeof(int));
    
    std::cout << "Скопировано 10 элементов массива" << std::endl;
    
    free(arr1);
    free(arr2);
}

int main(int argc, char* argv[]) {
    std::cout << "=== Программа для изучения ltrace ===" << std::endl;
    
    if (argc > 1) {
        std::string command = argv[1];
        
        if (command == "math") {
            math_operations();
        } else if (command == "string") {
            string_operations();
        } else if (command == "time") {
            time_operations();
        } else if (command == "memory") {
            memory_operations();
        } else if (command == "all") {
            math_operations();
            std::cout << std::endl;
            string_operations();
            std::cout << std::endl;
            time_operations();
            std::cout << std::endl;
            memory_operations();
        } else {
            std::cout << "Неизвестная команда. Используйте: math, string, time, memory, all" << std::endl;
        }
    } else {
        // Выполнить все операции по умолчанию
        math_operations();
        std::cout << std::endl;
        string_operations();
        std::cout << std::endl;
        time_operations();
        std::cout << std::endl;
        memory_operations();
    }
    
    return 0;
}

