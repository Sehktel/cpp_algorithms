/*
 * Демонстрация: Буферизация потоков в C++
 * 
 * Тема: Понимание буферизации и принудительная запись данных
 * 
 * Концепции:
 * - Буферизация потоков и её влияние на производительность
 * - Принудительная запись через flush() и endl
 * - Различия между unbuffered и buffered I/O
 * - Timing и синхронизация данных
 */

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

// Демонстрация влияния буферизации на вывод
void demonstrateBuffering() {
    cout << "\n=== Демонстрация буферизации ===" << endl;
    
    // Создаём файл для демонстрации
    ofstream file("buffering_demo.txt");
    
    if (!file.is_open()) {
        cerr << "Ошибка создания файла" << endl;
        return;
    }
    
    cout << "Записываем данные БЕЗ flush()..." << endl;
    
    for (int i = 1; i <= 5; ++i) {
        file << "Строка " << i << " (без flush)" << endl;
        
        // Симуляция задержки - демонстрирует влияние буферизации
        this_thread::sleep_for(chrono::milliseconds(500));
        
        // До flush данные могут находиться в буфере!
        cout << "Записано " << i << ", но файл может быть не обновлён" << endl;
    }
    
    cout << "\nТеперь принудительно сбрасываем буфер..." << endl;
    file.flush(); // Принудительная запись буфера
    cout << "✓ Буфер сброшен - все данные записаны на диск" << endl;
    
    file.close();
}

// Демонстрация различий между flush и endl
void demonstrateFlushVsEndl() {
    cout << "\n=== flush() vs endl ===" << endl;
    
    ofstream file1("flush_demo.txt");
    ofstream file2("endl_demo.txt");
    
    if (!file1.is_open() || !file2.is_open()) {
        cerr << "Ошибка создания файлов" << endl;
        return;
    }
    
    cout << "Сравнение flush() и endl:" << endl;
    
    // Использование flush - только сброс буфера
    file1 << "Строка с flush" << flush;
    file1 << "\nВторая строка с flush" << flush;
    
    // Использование endl - добавление новой строки + сброс буфера
    file2 << "Строка с endl" << endl;
    file2 << "Вторая строка с endl" << endl;
    
    cout << "✓ flush() - только сброс буфера, без добавления \\n" << endl;
    cout << "✓ endl - добавление \\n + сброс буфера" << endl;
    
    file1.close();
    file2.close();
}

// Демонстрация unitbuf флага для unbuffered вывода
void demonstrateUnitbuf() {
    cout << "\n=== Unbuffered I/O (unitbuf) ===" << endl;
    
    ofstream file("unbuffered_demo.txt");
    
    if (!file.is_open()) {
        cerr << "Ошибка создания файла" << endl;
        return;
    }
    
    cout << "Обычный режим (буферизованный):" << endl;
    file << "Тест 1 (не записан сразу)" << "\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "Задержка 1 сек - данные в буфере" << endl;
    
    cout << "\nВключаем unitbuf (unbuffered):" << endl;
    file << unitbuf; // Включаем немедленную запись
    
    file << "Тест 2 (записан немедленно)" << "\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "Задержка 1 сек - данные уже на диске!" << endl;
    
    cout << "Отключаем unitbuf:" << endl;
    file << nounitbuf; // Возвращаемся к буферизованному режиму
    
    file.close();
}

// Профессиональная техника: измерение производительности
void benchmarkBufferingPerformance() {
    cout << "\n=== Бенчмарк производительности буферизации ===" << endl;
    
    const int iterations = 1000;
    const string testData = "Это тестовая строка для измерения производительности буферизации ";
    
    // Тест 1: Без принудительного flush (только в конце)
    auto start = chrono::high_resolution_clock::now();
    
    ofstream file1("performance_test1.txt");
    for (int i = 0; i < iterations; ++i) {
        file1 << testData << i << "\n";
        // НЕТ flush() - данные накапливаются в буфере
    }
    file1.flush(); // Только один flush в конце
    auto end1 = chrono::high_resolution_clock::now();
    
    // Тест 2: С flush после каждой записи
    ofstream file2("performance_test2.txt");
    file2 << unitbuf; // Unbuffered режим
    
    auto start2 = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        file2 << testData << i << "\n";
        // Каждая запись немедленно сбрасывается
    }
    auto end2 = chrono::high_resolution_clock::now();
    
    // Вычисляем время выполнения
    auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start);
    auto duration2 = chrono::duration_cast<chrono::microseconds>(end2 - start2);
    
    cout << "Буферизованный режим: " << duration1.count() << " микросекунд" << endl;
    cout << "Unbuffered режим: " << duration2.count() << " микросекунд" << endl;
    
    if (duration1 < duration2) {
        cout << "✓ Буферизация быстрее в " << (double)duration2.count() / duration1.count() << " раз" << endl;
    }
    
    file1.close();
    file2.close();
}

int main() {
    cout << "=== ДЕМОНСТРАЦИЯ БУФЕРИЗАЦИИ ПОТОКОВ ===" << endl;
    
    // 1. Базовое понимание буферизации
    demonstrateBuffering();
    
    // 2. Различия между flush и endl
    demonstrateFlushVsEndl();
    
    // 3. Unbuffered I/O
    demonstrateUnitbuf();
    
    // 4. Производительность
    benchmarkBufferingPerformance();
    
    cout << "\n=== ВЫВОДЫ ДЛЯ ПРОДВИНУТЫХ РАЗРАБОТЧИКОВ ===" << endl;
    cout << "1. Буферизация критически важна для производительности I/O" << endl;
    cout << "2. Используйте flush() только когда данные должны быть немедленно записаны" << endl;
    cout << "3. endl дороже простого '\\n' из-за дополнительного flush" << endl;
    cout << "4. unitbuf полезен для логирования, но снижает производительность" << endl;
    
    return 0;
}
