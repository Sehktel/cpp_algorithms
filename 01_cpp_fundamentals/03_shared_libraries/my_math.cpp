#include "my_math.h"
#include <stdexcept>

// Реализация математических операций

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    // Проверка деления на ноль
    if (b == 0) {
        throw std::runtime_error("Деление на ноль невозможно");
    }
    return a / b;
} 