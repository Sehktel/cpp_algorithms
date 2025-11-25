// ⚠️ LEGACY: Этот файл мигрирован в 01_cpp_fundamentals/07_file_io/06_high_performance_io.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 01_cpp_fundamentals/07_file_io/06_high_performance_io.cpp

/*
 * Демонстрация: Высокопроизводительный I/O в C++
 * 
 * Тема: Оптимизация производительности файлового I/O
 * 
 * Концепции:
 * - sync_with_stdio(false) - отключение синхронизации с C stdio
 * - Memory-mapped files для больших файлов
 * - Блочное чтение/запись для максимальной производительности
 * - Профилирование I/O операций
 */

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <memoryapi.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

using namespace std;

// Генерация тестовых данных
vector<int> generateTestData(size_t count) {
    vector<int> data;
    data.reserve(count);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(INT_MIN, INT_MAX);
    
    for (size_t i = 0; i < count; ++i) {
        data.push_back(dis(gen));
    }
    
    return data;
}

// Демонстрация sync_with_stdio(false)
void demonstrateSyncWithStdio() {
    cout << "\n=== sync_with_stdio(false) демонстрация ===" << endl;
    
    const size_t dataSize = 1000000;
    auto testData = generateTestData(dataSize);
    
    // Тест 1: Обычная синхронизация (по умолчанию)
    cout << "Тест с синхронизацией (по умолчанию):" << endl;
    auto start = chrono::high_resolution_clock::now();
    
    ofstream file1("sync_test_default.txt", ios::binary);
    for (const auto& value : testData) {
        file1 << value << "\n";
    }
    auto end1 = chrono::high_resolution_clock::now();
    
    // Тест 2: Отключённая синхронизация
    cout << "Тест БЕЗ синхронизации:" << endl;
    ios_base::sync_with_stdio(false);
    
    auto start2 = chrono::high_resolution_clock::now();
    ofstream file2("sync_test_disabled.txt", ios::binary);
    for (const auto& value : testData) {
        file2 << value << "\n";
    }
    auto end2 = chrono::high_resolution_clock::now();
    
    // Восстанавливаем синхронизацию
    ios_base::sync_with_stdio(true);
    
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start);
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2);
    
    cout << "С синхронизацией: " << duration1.count() << " мс" << endl;
    cout << "Без синхронизации: " << duration2.count() << " мс" << endl;
    
    if (duration2 < duration1) {
        cout << "✓ Ускорение: " << (double)duration1.count() / duration2.count() << "x" << endl;
    }
    
    file1.close();
    file2.close();
}

// Блочное чтение/запись для максимальной производительности
void demonstrateBlockIO() {
    cout << "\n=== Блочный I/O ===" << endl;
    
    // Создаём тестовый файл
    const size_t fileSize = 10 * 1024 * 1024; // 10 MB
    vector<char> buffer(fileSize, 'X');
    
    // Записываем тестовые данные
    ofstream outFile("block_test.dat", ios::binary);
    outFile.write(buffer.data(), fileSize);
    outFile.close();
    
    // Метод 1: Построчное чтение (медленно)
    cout << "Построчное чтение:" << endl;
    ifstream inFile1("block_test.dat", ios::binary);
    
    auto start = chrono::high_resolution_clock::now();
    string line;
    size_t linesRead = 0;
    while (getline(inFile1, line)) {
        linesRead++;
    }
    auto end1 = chrono::high_resolution_clock::now();
    
    // Метод 2: Блочное чтение (быстро)
    cout << "Блочное чтение:" << endl;
    ifstream inFile2("block_test.dat", ios::binary);
    
    const size_t blockSize = 64 * 1024; // 64 KB блоки
    vector<char> readBuffer(blockSize);
    
    auto start2 = chrono::high_resolution_clock::now();
    size_t totalRead = 0;
    while (inFile2.read(readBuffer.data(), blockSize)) {
        totalRead += inFile2.gcount();
    }
    // Читаем остаток файла
    if (inFile2.gcount() > 0) {
        totalRead += inFile2.gcount();
    }
    auto end2 = chrono::high_resolution_clock::now();
    
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start);
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2);
    
    cout << "Построчное чтение: " << duration1.count() << " мс" << endl;
    cout << "Блочное чтение: " << duration2.count() << " мс" << endl;
    cout << "Прочитано байт блоками: " << totalRead << endl;
    
    inFile1.close();
    inFile2.close();
}

#ifdef _WIN32
// Memory-mapped files для Windows
class MemoryMappedFile {
private:
    HANDLE hFile;
    HANDLE hMapping;
    void* mappedData;
    size_t fileSize;

public:
    MemoryMappedFile(const string& filename) : hFile(INVALID_HANDLE_VALUE), 
                                               hMapping(nullptr), 
                                               mappedData(nullptr), 
                                               fileSize(0) {
        // Открываем файл
        hFile = CreateFileA(filename.c_str(), 
                           GENERIC_READ | GENERIC_WRITE,
                           0, nullptr, OPEN_EXISTING, 
                           FILE_ATTRIBUTE_NORMAL, nullptr);
        
        if (hFile == INVALID_HANDLE_VALUE) {
            throw runtime_error("Не удалось открыть файл");
        }
        
        // Получаем размер файла
        LARGE_INTEGER size;
        GetFileSizeEx(hFile, &size);
        fileSize = size.QuadPart;
        
        // Создаём mapping
        hMapping = CreateFileMapping(hFile, nullptr, PAGE_READWRITE, 0, 0, nullptr);
        if (!hMapping) {
            CloseHandle(hFile);
            throw runtime_error("Не удалось создать memory mapping");
        }
        
        // Маппируем файл в память
        mappedData = MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!mappedData) {
            CloseHandle(hMapping);
            CloseHandle(hFile);
            throw runtime_error("Не удалось отобразить файл в память");
        }
    }
    
    ~MemoryMappedFile() {
        if (mappedData) UnmapViewOfFile(mappedData);
        if (hMapping) CloseHandle(hMapping);
        if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
    }
    
    void* data() const { return mappedData; }
    size_t size() const { return fileSize; }
};

void demonstrateMemoryMappedIO() {
    cout << "\n=== Memory-mapped I/O (Windows) ===" << endl;
    
    // Создаём большой файл для тестирования
    const size_t fileSize = 50 * 1024 * 1024; // 50 MB
    const string filename = "mmap_test.dat";
    
    cout << "Создаём тестовый файл " << fileSize / (1024*1024) << " MB..." << endl;
    
    ofstream outFile(filename, ios::binary);
    vector<char> buffer(1024 * 1024, 'M'); // 1 MB буфер
    for (size_t i = 0; i < fileSize / (1024 * 1024); ++i) {
        outFile.write(buffer.data(), buffer.size());
    }
    outFile.close();
    
    try {
        MemoryMappedFile mmap(filename);
        cout << "✓ Файл отображён в память" << endl;
        cout << "Размер: " << mmap.size() / (1024*1024) << " MB" << endl;
        
        // Теперь можем работать с данными как с массивом в памяти
        char* data = static_cast<char*>(mmap.data());
        
        // Простой тест - подсчёт символов 'M'
        auto start = chrono::high_resolution_clock::now();
        size_t count = 0;
        for (size_t i = 0; i < mmap.size(); ++i) {
            if (data[i] == 'M') count++;
        }
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Найдено символов 'M': " << count << endl;
        cout << "Время обработки: " << duration.count() << " мс" << endl;
        
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}
#endif

int main() {
    cout << "=== ВЫСОКОПРОИЗВОДИТЕЛЬНЫЙ I/O В C++ ===" << endl;
    
    // 1. Оптимизация синхронизации
    demonstrateSyncWithStdio();
    
    // 2. Блочный I/O
    demonstrateBlockIO();
    
#ifdef _WIN32
    // 3. Memory-mapped files (только для Windows в данном примере)
    demonstrateMemoryMappedIO();
#endif
    
    cout << "\n=== КЛЮЧЕВЫЕ ПРИНЦИПЫ ОПТИМИЗАЦИИ ===" << endl;
    cout << "1. sync_with_stdio(false) ускоряет iostream операции" << endl;
    cout << "2. Блочное чтение/запись эффективнее построчного" << endl;
    cout << "3. Memory-mapped files идеальны для файлов >100MB" << endl;
    cout << "4. Измеряйте производительность перед оптимизацией!" << endl;
    cout << "\nВНИМАНИЕ: sync_with_stdio(false) нарушает совместимость" << endl;
    cout << "с printf/scanf функциями - используйте аккуратно!" << endl;
    
    return 0;
}
