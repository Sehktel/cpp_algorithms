/*
 * Пример программы с уязвимостью переполнения буфера
 * 
 * ⚠️ ВНИМАНИЕ: Эта программа создана только для учебных целей!
 * В реальных проектах такие уязвимости должны быть исправлены.
 * 
 * Используйте эту программу для изучения:
 * - Как GDB показывает переполнение буфера
 * - Анализ стека при переполнении
 * - Понимание механизмов защиты (stack canaries, ASLR)
 */

#include <iostream>
#include <cstring>

// Функция с уязвимостью переполнения буфера
void vulnerable_function(char* input) {
    char buffer[64];
    
    // ⚠️ КРИТИЧЕСКАЯ УЯЗВИМОСТЬ: strcpy не проверяет длину
    strcpy(buffer, input);
    
    std::cout << "Буфер содержит: " << buffer << std::endl;
}

// Безопасная версия функции
void safe_function(char* input, size_t max_len) {
    char buffer[64];
    
    // Безопасное копирование с проверкой длины
    size_t len = strlen(input);
    if (len >= max_len) {
        len = max_len - 1;
    }
    strncpy(buffer, input, len);
    buffer[len] = '\0';
    
    std::cout << "Безопасный буфер содержит: " << buffer << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <строка>" << std::endl;
        std::cout << "Попробуйте строку длиной более 64 символов для демонстрации переполнения" << std::endl;
        return 1;
    }
    
    char* input = argv[1];
    size_t input_len = strlen(input);
    
    std::cout << "Длина входной строки: " << input_len << " символов" << std::endl;
    std::cout << "Размер буфера: 64 байта" << std::endl;
    
    if (input_len > 64) {
        std::cout << "⚠️ ВНИМАНИЕ: Строка длиннее буфера! Возможно переполнение." << std::endl;
    }
    
    // Вызов уязвимой функции
    std::cout << "\n=== Вызов уязвимой функции ===" << std::endl;
    vulnerable_function(input);
    
    // Вызов безопасной функции
    std::cout << "\n=== Вызов безопасной функции ===" << std::endl;
    safe_function(input, 64);
    
    return 0;
}

