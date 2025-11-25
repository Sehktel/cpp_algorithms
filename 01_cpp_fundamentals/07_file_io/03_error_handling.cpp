/*
 * Лабораторная работа #3: Обработка ошибок и состояния потоков
 * 
 * Тема: Профессиональная обработка ошибок при работе с файлами
 * 
 * Состояния потока (stream state):
 * - goodbit (0x00): всё в порядке
 * - eofbit  (0x01): достигнут конец файла
 * - failbit (0x02): логическая ошибка (неверный формат данных)
 * - badbit  (0x04): критическая ошибка (повреждение потока)
 * 
 * Эти биты составляют iostate - битовую маску состояния.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// Класс-обёртка для безопасной работы с файлами
class SafeFileReader {
private:
    ifstream file;
    string filename;
    
public:
    explicit SafeFileReader(const string& fname) : filename(fname) {
        /*
         * Открываем файл с проверкой исключений
         * По умолчанию потоки C++ НЕ генерируют исключения!
         * Нужно явно включить через exceptions()
         */
        file.exceptions(ifstream::badbit); // Исключение только на критические ошибки
        
        try {
            file.open(filename);
        } catch (const ios_base::failure& e) {
            cerr << "Критическая ошибка открытия файла: " << e.what() << endl;
            throw; // Перебрасываем дальше
        }
    }
    
    // Проверка успешности открытия
    bool isOpen() const {
        return file.is_open();
    }
    
    // Чтение с валидацией состояния потока
    bool readLine(string& line) {
        if (getline(file, line)) {
            return true;
        }
        
        // Анализируем причину неудачи
        if (file.eof()) {
            cout << "Достигнут конец файла" << endl;
            return false;
        }
        
        if (file.fail()) {
            cerr << "Логическая ошибка чтения (failbit установлен)" << endl;
            /*
             * clear() сбрасывает флаги ошибок
             * После этого можно попытаться продолжить работу
             */
            file.clear();
            return false;
        }
        
        if (file.bad()) {
            cerr << "Критическая ошибка потока (badbit установлен)" << endl;
            return false;
        }
        
        return false;
    }
    
    // Безопасное чтение числа с валидацией
    bool readInteger(int& value) {
        file >> value;
        
        if (file.good()) {
            // Пропускаем оставшуюся часть строки
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        
        if (file.fail()) {
            cerr << "Ошибка: введено не целое число" << endl;
            // Очищаем состояние ошибки
            file.clear();
            // Очищаем некорректный ввод из буфера
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }
        
        return false;
    }
    
    // Получение текущей позиции в файле
    streampos tell() const {
        return const_cast<ifstream&>(file).tellg();
    }
    
    ~SafeFileReader() {
        if (file.is_open()) {
            file.close();
            cout << "✓ Файл " << filename << " безопасно закрыт" << endl;
        }
    }
};

// Демонстрация различных сценариев ошибок
void demonstrateErrorScenarios() {
    cout << "=== Сценарий 1: Несуществующий файл ===" << endl;
    {
        ifstream file("nonexistent.txt");
        if (!file) { // operator bool() проверяет good()
            cerr << "✗ Файл не найден (как и ожидалось)" << endl;
            cerr << "  fail() = " << file.fail() << endl;
            cerr << "  bad()  = " << file.bad() << endl;
        }
    }
    
    cout << "\n=== Сценарий 2: Неверный формат данных ===" << endl;
    {
        // Создаём файл с текстом, а не числом
        ofstream out("test_format.txt");
        out << "это_не_число" << endl;
        out.close();
        
        ifstream in("test_format.txt");
        int number;
        in >> number;
        
        if (in.fail()) {
            cerr << "✗ Ошибка парсинга (как и ожидалось)" << endl;
            cerr << "  failbit установлен: " << in.fail() << endl;
            
            // Восстанавливаем поток
            in.clear();
            string actualContent;
            in >> actualContent;
            cout << "  Фактическое содержимое: " << actualContent << endl;
        }
    }
    
    cout << "\n=== Сценарий 3: Использование исключений ===" << endl;
    {
        try {
            ifstream file;
            // Включаем исключения для всех ошибок
            file.exceptions(ifstream::failbit | ifstream::badbit);
            file.open("nonexistent_with_exceptions.txt");
            
        } catch (const ifstream::failure& e) {
            cerr << "✗ Поймано исключение: " << e.what() << endl;
            cerr << "  Код ошибки: " << e.code() << endl;
        }
    }
}

// Робастное чтение конфигурационного файла
struct Config {
    string appName;
    int maxConnections;
    double timeout;
    bool enableLogging;
};

bool readConfig(const string& filename, Config& config) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Не удалось открыть конфигурационный файл: " << filename << endl;
        return false;
    }
    
    string line;
    int lineNumber = 0;
    
    while (getline(file, line)) {
        lineNumber++;
        
        // Пропускаем комментарии и пустые строки
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Парсим формат "ключ=значение"
        size_t pos = line.find('=');
        if (pos == string::npos) {
            cerr << "Предупреждение: неверный формат в строке " << lineNumber << endl;
            continue;
        }
        
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        
        // Заполняем структуру
        try {
            if (key == "appName") {
                config.appName = value;
            } else if (key == "maxConnections") {
                config.maxConnections = stoi(value);
            } else if (key == "timeout") {
                config.timeout = stod(value);
            } else if (key == "enableLogging") {
                config.enableLogging = (value == "true" || value == "1");
            } else {
                cerr << "Предупреждение: неизвестный ключ '" << key 
                     << "' в строке " << lineNumber << endl;
            }
        } catch (const invalid_argument& e) {
            cerr << "Ошибка преобразования значения для ключа '" << key 
                 << "' в строке " << lineNumber << endl;
            return false;
        } catch (const out_of_range& e) {
            cerr << "Значение вне допустимого диапазона для ключа '" << key 
                 << "' в строке " << lineNumber << endl;
            return false;
        }
    }
    
    // Проверяем, закончили ли мы из-за ошибки или EOF
    if (file.bad()) {
        cerr << "Критическая ошибка чтения файла" << endl;
        return false;
    }
    
    return true;
}

int main() {
    // Демонстрация сценариев ошибок
    demonstrateErrorScenarios();
    
    cout << "\n=== Тест SafeFileReader ===" << endl;
    
    // Создаём тестовый файл
    {
        ofstream out("test_data.txt");
        out << "42\n";
        out << "Строка текста\n";
        out << "не_число\n";
        out << "100\n";
    }
    
    try {
        SafeFileReader reader("test_data.txt");
        
        if (!reader.isOpen()) {
            cerr << "Не удалось открыть файл" << endl;
            return 1;
        }
        
        int num;
        string line;
        
        // Читаем первое число
        if (reader.readInteger(num)) {
            cout << "Прочитано число: " << num << endl;
        }
        
        // Читаем строку
        if (reader.readLine(line)) {
            cout << "Прочитана строка: " << line << endl;
        }
        
        // Пытаемся прочитать число (будет ошибка)
        if (!reader.readInteger(num)) {
            cout << "Ошибка чтения числа обработана корректно" << endl;
        }
        
        // Читаем следующее число после ошибки
        if (reader.readInteger(num)) {
            cout << "Прочитано число после восстановления: " << num << endl;
        }
        
    } catch (const exception& e) {
        cerr << "Исключение: " << e.what() << endl;
        return 1;
    }
    
    // Демонстрация чтения конфига
    cout << "\n=== Чтение конфигурационного файла ===" << endl;
    
    {
        ofstream out("config.ini");
        out << "# Конфигурация приложения\n";
        out << "appName=FileIOLab\n";
        out << "maxConnections=100\n";
        out << "timeout=30.5\n";
        out << "enableLogging=true\n";
    }
    
    Config config;
    if (readConfig("config.ini", config)) {
        cout << "✓ Конфигурация загружена успешно:" << endl;
        cout << "  Имя приложения: " << config.appName << endl;
        cout << "  Макс. соединений: " << config.maxConnections << endl;
        cout << "  Таймаут: " << config.timeout << endl;
        cout << "  Логирование: " << (config.enableLogging ? "включено" : "выключено") << endl;
    }
    
    /*
     * ФИЛОСОФСКОЕ ЗАМЕЧАНИЕ:
     * 
     * В C++ существует два подхода к обработке ошибок:
     * 
     * 1. Коды возврата (error codes) - традиционный C-стиль
     *    + Явная обработка, предсказуемая производительность
     *    - Легко забыть проверку, код становится verbose
     * 
     * 2. Исключения (exceptions) - современный C++ стиль
     *    + Невозможно игнорировать, чистый код для happy path
     *    - Overhead при генерации, сложность отслеживания flow
     * 
     * Для файлового I/O обычно используют гибридный подход:
     * - Проверка is_open() для предсказуемых ошибок
     * - Исключения для критических сбоев
     * 
     * В высоконагруженных системах (HFT, game engines) предпочитают
     * коды возврата из-за детерминированной производительности.
     */
    
    return 0;
}


