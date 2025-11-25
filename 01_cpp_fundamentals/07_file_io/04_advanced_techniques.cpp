/*
 * Лабораторная работа #4: Продвинутые техники
 * 
 * Тема: Производительность, buffering, манипуляторы потоков
 * 
 * Эта лабораторная демонстрирует:
 * - Настройку буферизации
 * - Использование манипуляторов
 * - Оптимизацию производительности
 * - Работу с большими файлами
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>

using namespace std;
using namespace chrono;

// Утилита для измерения времени выполнения
class Timer {
    time_point<high_resolution_clock> start;
    string label;
    
public:
    explicit Timer(const string& lbl) : label(lbl) {
        start = high_resolution_clock::now();
    }
    
    ~Timer() {
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "⏱ " << label << ": " << duration.count() << " мс" << endl;
    }
};

// Демонстрация манипуляторов потоков
void demonstrateManipulators() {
    cout << "=== Демонстрация манипуляторов ===" << endl;
    
    ofstream file("formatted_output.txt");
    
    /*
     * Манипуляторы - функции, изменяющие состояние потока
     * Определены в <iomanip>
     * 
     * Две категории:
     * 1. С параметрами: setw(), setprecision(), setfill()
     * 2. Без параметров: endl, fixed, scientific, hex, oct, dec
     * 
     * Важно: setw() действует только на следующую операцию!
     * Остальные манипуляторы сохраняются (sticky).
     */
    
    double pi = 3.14159265358979323846;
    int number = 42;
    
    file << "=== Форматирование чисел ===" << endl;
    
    // Управление точностью
    file << "По умолчанию: " << pi << endl;
    file << "Точность 3: " << setprecision(3) << pi << endl;
    file << "Fixed нотация: " << fixed << setprecision(2) << pi << endl;
    file << "Scientific: " << scientific << setprecision(4) << pi << endl;
    file << defaultfloat; // Возврат к умолчанию
    
    file << "\n=== Выравнивание и ширина ===" << endl;
    
    // Выравнивание с шириной поля
    file << left << setw(20) << "Имя" << setw(10) << "Возраст" << endl;
    file << setw(20) << "Александр" << setw(10) << 25 << endl;
    file << setw(20) << "Мария" << setw(10) << 30 << endl;
    
    file << "\n=== Заполнение символами ===" << endl;
    
    file << setfill('*') << setw(40) << "" << endl; // Линия из звёздочек
    file << setfill(' '); // Возврат к пробелам
    
    file << "\n=== Системы счисления ===" << endl;
    
    file << "Десятичная: " << dec << number << endl;
    file << "Шестнадцатеричная: 0x" << hex << number << endl;
    file << "Восьмеричная: 0" << oct << number << endl;
    file << dec; // Возврат к десятичной
    
    file << "\n=== Булевы значения ===" << endl;
    
    file << "По умолчанию: " << true << endl;
    file << "С boolalpha: " << boolalpha << true << endl;
    file << noboolalpha; // Возврат к цифровому формату
    
    cout << "✓ Форматированный вывод записан в formatted_output.txt" << endl;
}

// Сравнение производительности различных подходов
void performanceComparison(int numLines) {
    cout << "\n=== Сравнение производительности ===" << endl;
    cout << "Запись " << numLines << " строк...\n" << endl;
    
    const string testData = "Строка данных для тестирования производительности записи в файл\n";
    
    // Метод 1: Обычная запись с endl (включает flush)
    {
        Timer timer("Метод 1: с endl (flush)");
        ofstream file("perf_test1.txt");
        for (int i = 0; i < numLines; i++) {
            file << testData << endl; // endl делает flush!
        }
    }
    
    // Метод 2: Запись с \n (без flush)
    {
        Timer timer("Метод 2: с \\n (без flush)");
        ofstream file("perf_test2.txt");
        for (int i = 0; i < numLines; i++) {
            file << testData << "\n"; // Намного быстрее!
        }
    }
    
    // Метод 3: Буферизованная запись с увеличенным буфером
    {
        Timer timer("Метод 3: увеличенный буфер");
        ofstream file("perf_test3.txt");
        
        /*
         * rdbuf()->pubsetbuf(buffer, size) устанавливает пользовательский буфер
         * По умолчанию размер буфера ~8KB
         * Увеличение до 64KB может дать существенный прирост
         */
        const int bufferSize = 65536; // 64KB
        char* buffer = new char[bufferSize];
        file.rdbuf()->pubsetbuf(buffer, bufferSize);
        
        for (int i = 0; i < numLines; i++) {
            file << testData << "\n";
        }
        
        delete[] buffer;
    }
    
    // Метод 4: Запись большими блоками
    {
        Timer timer("Метод 4: блочная запись");
        ofstream file("perf_test4.txt");
        
        // Собираем данные в памяти
        string chunk;
        chunk.reserve(testData.size() * 1000); // Резервируем память
        
        for (int i = 0; i < numLines; i++) {
            chunk += testData;
            
            // Пишем блоками по 1000 строк
            if ((i + 1) % 1000 == 0) {
                file << chunk;
                chunk.clear();
            }
        }
        
        // Записываем остаток
        if (!chunk.empty()) {
            file << chunk;
        }
    }
    
    /*
     * РЕЗУЛЬТАТЫ БЕНЧМАРКА (типичные на современном SSD):
     * 
     * 100,000 строк:
     * - Метод 1 (endl):       ~500-800 мс  ❌ Медленно
     * - Метод 2 (\n):         ~100-150 мс  ✓ Хорошо
     * - Метод 3 (большой buf): ~80-120 мс  ✓✓ Лучше
     * - Метод 4 (блоки):       ~50-80 мс   ✓✓✓ Отлично
     * 
     * ВЫВОДЫ:
     * 1. НИКОГДА не используйте endl в циклах!
     * 2. Увеличение буфера даёт 20-30% прироста
     * 3. Блочная запись - оптимальная стратегия
     * 4. Для huge файлов (GB+) рассмотрите memory mapping
     */
}

// Работа с CSV файлами (типичная задача)
class CSVWriter {
    ofstream file;
    bool firstColumn;
    
public:
    CSVWriter(const string& filename) : firstColumn(true) {
        file.open(filename);
        if (!file.is_open()) {
            throw runtime_error("Не удалось создать CSV файл");
        }
    }
    
    // Добавление значения в строку
    template<typename T>
    CSVWriter& add(const T& value) {
        if (!firstColumn) {
            file << ",";
        }
        file << value;
        firstColumn = false;
        return *this; // Позволяет цепочечные вызовы
    }
    
    // Завершение строки
    void endRow() {
        file << "\n";
        firstColumn = true;
    }
    
    ~CSVWriter() {
        if (file.is_open()) {
            file.close();
        }
    }
};

void demonstrateCSV() {
    cout << "\n=== Работа с CSV ===" << endl;
    
    CSVWriter csv("data.csv");
    
    // Заголовок
    csv.add("ID").add("Имя").add("Возраст").add("Зарплата");
    csv.endRow();
    
    // Данные
    csv.add(1).add("Иванов И.И.").add(30).add(75000.50);
    csv.endRow();
    
    csv.add(2).add("Петрова М.С.").add(25).add(65000.00);
    csv.endRow();
    
    csv.add(3).add("Сидоров П.К.").add(35).add(85000.75);
    csv.endRow();
    
    cout << "✓ CSV файл создан" << endl;
}

// Чтение большого файла по частям (streaming)
void processLargeFileStreaming(const string& filename) {
    cout << "\n=== Потоковая обработка большого файла ===" << endl;
    
    // Создаём тестовый "большой" файл
    {
        ofstream out(filename);
        for (int i = 0; i < 1000000; i++) {
            out << "Строка номер " << i << "\n";
        }
    }
    
    Timer timer("Обработка 1,000,000 строк");
    
    ifstream file(filename);
    string line;
    int lineCount = 0;
    long long totalChars = 0;
    
    /*
     * Потоковая обработка - ключевая концепция для Big Data
     * 
     * Преимущества:
     * - O(1) память (константное использование)
     * - Начинаем обработку немедленно
     * - Масштабируется на файлы любого размера
     * 
     * Альтернатива - загрузка в память:
     * - Требует O(n) памяти
     * - Не масштабируется
     * - Может быть быстрее для малых файлов (лучше locality)
     */
    
    while (getline(file, line)) {
        lineCount++;
        totalChars += line.length();
        
        // Здесь могла бы быть реальная обработка:
        // - Парсинг
        // - Фильтрация
        // - Агрегация
        // - Трансформация и запись в другой файл
    }
    
    cout << "Обработано строк: " << lineCount << endl;
    cout << "Всего символов: " << totalChars << endl;
}

int main() {
    // Демонстрация 1: Манипуляторы
    demonstrateManipulators();
    
    // Демонстрация 2: Производительность
    performanceComparison(100000);
    
    // Демонстрация 3: CSV
    demonstrateCSV();
    
    // Демонстрация 4: Большие файлы
    processLargeFileStreaming("large_file.txt");
    
    cout << "\n" << string(60, '=') << endl;
    cout << "КЛЮЧЕВЫЕ ВЫВОДЫ:" << endl;
    cout << string(60, '=') << endl;
    cout << "1. Избегайте endl в циклах - используйте \\n" << endl;
    cout << "2. Увеличивайте буфер для интенсивного I/O" << endl;
    cout << "3. Блочная запись эффективнее построчной" << endl;
    cout << "4. Потоковая обработка для больших данных" << endl;
    cout << "5. Манипуляторы - мощный инструмент форматирования" << endl;
    cout << string(60, '=') << endl;
    
    /*
     * ДОПОЛНИТЕЛЬНЫЕ ТЕМЫ ДЛЯ ИЗУЧЕНИЯ:
     * 
     * 1. Memory-Mapped Files (mmap на POSIX, CreateFileMapping на Windows)
     *    - Отображение файла в адресное пространство
     *    - Прямой доступ к данным без системных вызовов
     *    - Используется в БД (SQLite, RocksDB)
     * 
     * 2. Асинхронный I/O
     *    - io_uring (Linux)
     *    - IOCP (Windows)
     *    - Позволяет перекрывать I/O и вычисления
     * 
     * 3. Zero-copy техники
     *    - sendfile(), splice() на Linux
     *    - Передача данных без копирования в user space
     * 
     * 4. Сжатие на лету
     *    - zlib, lz4 для transparent compression
     *    - Баланс CPU/IO
     * 
     * 5. Distributed File Systems
     *    - HDFS для Hadoop
     *    - Ceph, GlusterFS
     *    - Совершенно иная парадигма
     */
    
    return 0;
}


