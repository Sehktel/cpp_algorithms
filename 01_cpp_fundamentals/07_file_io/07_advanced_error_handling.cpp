/*
 * Демонстрация: Расширенная обработка ошибок потоков
 * 
 * Тема: Профессиональная обработка ошибок в файловом I/O
 * 
 * Концепции:
 * - Состояния потоков: good(), fail(), bad(), eof()
 * - Битовые флаги состояния потока
 * - clear() и восстановление после ошибок
 * - Исключения в потоках и их обработка
 * - Пользовательские обработчики ошибок
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <exception>

using namespace std;

// Функция для детального анализа состояния потока
void analyzeStreamState(ostream& stream, const string& operation) {
    cout << "\n=== Анализ состояния потока после: " << operation << " ===" << endl;
    
    ios_base::iostate state = stream.rdstate();
    
    cout << "Состояние потока (hex): 0x" << hex << state << dec << endl;
    cout << "good(): " << boolalpha << stream.good() << " - поток в нормальном состоянии" << endl;
    cout << "fail(): " << boolalpha << stream.fail() << " - логическая ошибка операции" << endl;
    cout << "bad(): " << boolalpha << stream.bad() << " - критическая ошибка потока" << endl;
    cout << "eof(): " << boolalpha << stream.eof() << " - достигнут конец файла" << endl;
    
    // Детальный анализ битовых флагов
    cout << "\nДетальный анализ:" << endl;
    if (stream.good()) {
        cout << "✓ Все операции прошли успешно" << endl;
    } else {
        if (stream.eof()) {
            cout << "⚠ Достигнут конец потока (EOF)" << endl;
        }
        if (stream.fail()) {
            cout << "✗ Логическая ошибка (например, неверный формат данных)" << endl;
        }
        if (stream.bad()) {
            cout << "✗✗ Критическая ошибка потока (недоступность устройства)" << endl;
        }
    }
}

// Демонстрация различных типов ошибок
void demonstrateStreamErrors() {
    cout << "\n=== ДЕМОНСТРАЦИЯ ОШИБОК ПОТОКОВ ===" << endl;
    
    // Создаём тестовый файл
    {
        ofstream testFile("error_test.txt");
        testFile << "123 456.789 строка текста" << endl;
        testFile << "неверный_формат_числа" << endl;
    }
    
    ifstream inFile("error_test.txt");
    if (!inFile.is_open()) {
        cerr << "Не удалось открыть тестовый файл" << endl;
        return;
    }
    
    // Тест 1: Успешное чтение
    cout << "\n--- Тест 1: Нормальное чтение ---" << endl;
    int number1, number2;
    double dnumber;
    string text;
    
    inFile >> number1 >> dnumber >> text;
    analyzeStreamState(inFile, "чтение корректных данных");
    
    cout << "Прочитано: " << number1 << ", " << dnumber << ", " << text << endl;
    
    // Тест 2: Ошибка форматирования
    cout << "\n--- Тест 2: Ошибка форматирования ---" << endl;
    int invalidNumber;
    inFile >> invalidNumber; // Попытка прочитать "неверный_формат_числа" как int
    analyzeStreamState(inFile, "попытка чтения неверного формата");
    
    cout << "Попытка прочитать как int: " << invalidNumber << endl;
    
    // Тест 3: Чтение после ошибки
    cout << "\n--- Тест 3: Состояние после clear() ---" << endl;
    inFile.clear(); // Сбрасываем флаги ошибок
    analyzeStreamState(inFile, "clear()");
    
    // Тест 4: Достижение конца файла
    cout << "\n--- Тест 4: Достижение EOF ---" << endl;
    string lastLine;
    while (getline(inFile, lastLine)) {
        // Читаем до конца
    }
    analyzeStreamState(inFile, "достижение EOF");
    
    inFile.close();
}

// Профессиональная функция чтения с обработкой ошибок
template<typename T>
bool safeRead(istream& stream, T& value, const string& fieldName = "") {
    T originalValue = value;
    
    if (!stream.good()) {
        cerr << "Ошибка: поток не в корректном состоянии для чтения " 
             << fieldName << endl;
        return false;
    }
    
    stream >> value;
    
    if (stream.fail()) {
        stream.clear(); // Сбрасываем fail bit
        value = originalValue; // Восстанавливаем исходное значение
        
        cerr << "Ошибка: не удалось прочитать " << fieldName 
             << " - неверный формат данных" << endl;
        
        // Пропускаем проблемную строку
        stream.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    
    if (stream.bad()) {
        cerr << "Критическая ошибка при чтении " << fieldName << endl;
        return false;
    }
    
    return true;
}

// Демонстрация безопасного чтения
void demonstrateSafeReading() {
    cout << "\n=== БЕЗОПАСНОЕ ЧТЕНИЕ С ОБРАБОТКОЙ ОШИБОК ===" << endl;
    
    // Создаём файл с проблемными данными
    {
        ofstream testFile("safe_read_test.txt");
        testFile << "100 200.5 корректная_строка" << endl;
        testFile << "неверное_число 300.7 текст" << endl;  // Проблемная строка
        testFile << "150 400.2 ещё_строка" << endl;
    }
    
    ifstream inFile("safe_read_test.txt");
    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }
    
    string line;
    int lineNumber = 0;
    
    while (getline(inFile, line)) {
        lineNumber++;
        istringstream lineStream(line);
        
        cout << "\nОбработка строки " << lineNumber << ": " << line << endl;
        
        int intValue = 0;
        double doubleValue = 0.0;
        string stringValue;
        
        // Безопасное чтение каждого поля
        bool success1 = safeRead(lineStream, intValue, "целое число");
        bool success2 = safeRead(lineStream, doubleValue, "число с плавающей точкой");
        bool success3 = safeRead(lineStream, stringValue, "строка");
        
        if (success1 && success2 && success3) {
            cout << "✓ Успешно прочитано: " << intValue 
                 << ", " << doubleValue << ", " << stringValue << endl;
        } else {
            cout << "✗ Ошибка обработки строки " << lineNumber << endl;
        }
        
        // Очищаем состояние потока для следующей итерации
        lineStream.clear();
    }
    
    inFile.close();
}

// Класс для демонстрации exception-based обработки ошибок
class RobustFileReader {
private:
    ifstream file;
    string filename;
    
public:
    RobustFileReader(const string& name) : filename(name) {
        file.exceptions(ifstream::badbit); // Включаем исключения только для критических ошибок
        file.open(filename);
        
        if (!file.is_open()) {
            throw runtime_error("Не удалось открыть файл: " + filename);
        }
    }
    
    ~RobustFileReader() {
        if (file.is_open()) {
            file.close();
        }
    }
    
    bool readLineSafely(string& line) {
        try {
            // Пытаемся прочитать строку
            if (!getline(file, line)) {
                if (file.eof()) {
                    return false; // Естественный конец файла
                } else {
                    throw runtime_error("Ошибка чтения строки из " + filename);
                }
            }
            return true;
        } catch (const ios_base::failure& e) {
            cerr << "I/O ошибка при чтении " << filename << ": " << e.what() << endl;
            return false;
        }
    }
    
    void resetStream() {
        file.clear(); // Сбрасываем все флаги ошибок
    }
};

void demonstrateExceptionHandling() {
    cout << "\n=== ОБРАБОТКА ИСКЛЮЧЕНИЙ В ПОТОКАХ ===" << endl;
    
    try {
        RobustFileReader reader("safe_read_test.txt");
        string line;
        int lineCount = 0;
        
        while (reader.readLineSafely(line)) {
            lineCount++;
            cout << "Строка " << lineCount << ": " << line << endl;
        }
        
        cout << "✓ Файл прочитан полностью. Всего строк: " << lineCount << endl;
        
    } catch (const exception& e) {
        cerr << "Исключение: " << e.what() << endl;
    }
}

int main() {
    cout << "=== РАСШИРЕННАЯ ОБРАБОТКА ОШИБОК ПОТОКОВ ===" << endl;
    
    // 1. Анализ состояний потоков
    demonstrateStreamErrors();
    
    // 2. Безопасное чтение с обработкой ошибок
    demonstrateSafeReading();
    
    // 3. Исключения в потоках
    demonstrateExceptionHandling();
    
    cout << "\n=== ПРИНЦИПЫ ПРОФЕССИОНАЛЬНОЙ ОБРАБОТКИ ОШИБОК ===" << endl;
    cout << "1. ВСЕГДА проверяйте состояние потока перед операциями" << endl;
    cout << "2. Различайте fail() (логические ошибки) и bad() (критические)" << endl;
    cout << "3. Используйте clear() для восстановления после recoverable ошибок" << endl;
    cout << "4. Пропускайте некорректные данные с ignore()" << endl;
    cout << "5. Для критических ошибок используйте exceptions" << endl;
    cout << "6. Логируйте ошибки для отладки и мониторинга" << endl;
    
    return 0;
}
