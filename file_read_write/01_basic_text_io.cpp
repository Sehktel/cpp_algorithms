/*
 * Лабораторная работа #1: Базовый текстовый ввод/вывод
 * 
 * Тема: Фундаментальные операции с текстовыми файлами через fstream
 * 
 * Концепции:
 * - ofstream (output file stream) - для записи
 * - ifstream (input file stream) - для чтения
 * - RAII (Resource Acquisition Is Initialization) - автоматическое управление ресурсами
 */

// ⚠️ LEGACY: Этот файл мигрирован в 01_cpp_fundamentals/07_file_io/01_basic_text_io.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 01_cpp_fundamentals/07_file_io/01_basic_text_io.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Структуры из struct.cpp для демонстрации работы с комплексными данными
struct PERSON {   // Объявление структуры PERSON
    int age;       // Возраст
    long ss;       // Социальный номер
    float weight;  // Вес
    char name[25]; // Имя (фиксированный размер)
    
    // Конструктор для удобной инициализации
    PERSON(int a = 0, long s = 0, float w = 0.0, const char* n = "") 
        : age(a), ss(s), weight(w) {
        strncpy(name, n, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0'; // Гарантируем null-termination
    }
    
    // Метод для отображения информации о персоне
    void display() const {
        cout << "Персона: " << name 
             << ", Возраст: " << age 
             << ", ССН: " << ss 
             << ", Вес: " << weight << " кг" << endl;
    }
    
    // Метод для записи в текстовый поток
    void writeToStream(ofstream& out) const {
        out << name << " " << age << " " << ss << " " << weight << endl;
    }
    
    // Метод для чтения из текстового потока
    bool readFromStream(ifstream& in) {
        return (in >> name >> age >> ss >> weight);
    }
};

// Битовое поле для демонстрации работы с битовыми структурами
struct CELL {   // Объявление структуры CELL с битовыми полями
    unsigned short character  : 8;  // 00000000 ????????
    unsigned short foreground : 3;  // 00000??? 00000000
    unsigned short intensity  : 1;  // 0000?000 00000000
    unsigned short background : 3;  // 0???0000 00000000
    unsigned short blink      : 1;  // ?0000000 00000000
    
    // Конструктор
    CELL(unsigned char ch = 0, unsigned char fg = 0, bool intense = false, 
         unsigned char bg = 0, bool blnk = false) 
        : character(ch), foreground(fg), intensity(intense), 
          background(bg), blink(blnk) {}
    
    // Метод для отображения состояния ячейки
    void display() const {
        cout << "CELL: символ=" << (int)character 
             << ", передний план=" << (int)foreground 
             << ", яркость=" << intensity 
             << ", фон=" << (int)background 
             << ", мигание=" << blink << endl;
    }
};

// Функция демонстрирует запись в файл
void writeToFile(const string& filename) {
    /*
     * ofstream автоматически создаёт файл, если он не существует
     * При выходе из области видимости деструктор автоматически закрывает файл
     * Это демонстрация идиомы RAII - один из краеугольных камней C++
     */
    ofstream outFile(filename);
    
    // Проверка успешности открытия - критически важно!
    if (!outFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для записи: " << filename << endl;
        return;
    }
    
    // Запись данных различных типов
    outFile << "=== Демонстрация записи в файл ===" << endl;
    outFile << "Строка текста" << endl;
    outFile << "Целое число: " << 42 << endl;
    outFile << "Число с плавающей точкой: " << 3.14159 << endl;
    outFile << "Булево значение: " << boolalpha << true << endl;
    
    cout << "✓ Данные успешно записаны в " << filename << endl;
    // outFile автоматически закроется при выходе из функции
}

// Функция демонстрирует чтение из файла построчно
void readFromFile(const string& filename) {
    ifstream inFile(filename);
    
    if (!inFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для чтения: " << filename << endl;
        return;
    }
    
    cout << "\n=== Содержимое файла ===" << endl;
    
    string line;
    /*
     * getline() читает до символа новой строки
     * Это предпочтительный способ для построчного чтения текста
     * Альтернатива: operator>> - читает до пробела (whitespace-delimited)
     */
    while (getline(inFile, line)) {
        cout << line << endl;
    }
}

// Функция для записи структурированных данных (структуры PERSON)
void writeStructuredPersons(const string& filename) {
    ofstream outFile(filename, ios::app); // добавляем к существующему файлу
    
    if (!outFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для добавления: " << filename << endl;
        return;
    }
    
    cout << "\n=== Запись структурированных данных (PERSON) ===" << endl;
    
    // Создаём массив персон для демонстрации
    PERSON persons[] = {
        PERSON(25, 1234567890L, 70.5f, "Иванов"),
        PERSON(30, 9876543210L, 65.2f, "Петрова"),
        PERSON(45, 1122334455L, 80.0f, "Сидоров")
    };
    
    int count = sizeof(persons) / sizeof(persons[0]);
    
    // Записываем заголовок секции
    outFile << "\n=== СТРУКТУРИРОВАННЫЕ ДАННЫЕ PERSON ===" << endl;
    outFile << "Количество записей: " << count << endl;
    
    // Записываем каждую структуру в текстовом формате
    for (int i = 0; i < count; i++) {
        persons[i].writeToStream(outFile);
        persons[i].display(); // выводим на экран для подтверждения
    }
    
    cout << "✓ Записано " << count << " структур PERSON" << endl;
}

// Функция для чтения структурированных данных (структуры PERSON)
void readStructuredPersons(const string& filename) {
    ifstream inFile(filename);
    
    if (!inFile.is_open()) {
        cerr << "Ошибка: файл не найден" << endl;
        return;
    }
    
    cout << "\n=== Чтение структурированных данных (PERSON) ===" << endl;
    
    string line;
    bool foundSection = false;
    int recordCount = 0;
    
    // Ищем секцию с структурированными данными
    while (getline(inFile, line)) {
        if (line.find("=== СТРУКТУРИРОВАННЫЕ ДАННЫЕ PERSON ===") != string::npos) {
            foundSection = true;
            // Читаем следующую строку с количеством записей
            getline(inFile, line);
            cout << line << endl;
            
            // Парсим количество записей (упрощённо)
            if (line.find("Количество записей:") != string::npos) {
                size_t pos = line.find(": ");
                if (pos != string::npos) {
                    recordCount = stoi(line.substr(pos + 2));
                    cout << "Найдено записей для чтения: " << recordCount << endl;
                }
            }
            break;
        }
    }
    
    if (!foundSection) {
        cout << "Секция структурированных данных не найдена" << endl;
        return;
    }
    
    // Читаем структуры
    PERSON person;
    int readCount = 0;
    
    while (readCount < recordCount && person.readFromStream(inFile)) {
        cout << "Прочитана запись " << (readCount + 1) << ": ";
        person.display();
        readCount++;
    }
    
    cout << "✓ Прочитано " << readCount << " структур PERSON" << endl;
}

// Демонстрация работы с битовыми полями и записью бинарных данных
void demonstrateBitFields() {
    cout << "\n=== Демонстрация битовых полей (CELL) ===" << endl;
    
    // Создаём несколько ячеек экрана
    CELL cells[] = {
        CELL('A', 1, true, 0, false),   // символ A, красный фон, яркость
        CELL('H', 2, false, 1, true),   // символ H, зелёный фон, мигание
        CELL(0x2588, 3, true, 2, false) // блок, синий фон
    };
    
    int count = sizeof(cells) / sizeof(cells[0]);
    
    // Отображаем битовые поля
    for (int i = 0; i < count; i++) {
        cout << "Ячейка " << i << ": ";
        cells[i].display();
    }
    
    // Записываем битовые поля в файл (как текстовые данные)
    ofstream bitFile("bitfields.txt");
    if (bitFile.is_open()) {
        bitFile << "=== БИТОВЫЕ ПОЛЯ CELL ===" << endl;
        for (int i = 0; i < count; i++) {
            bitFile << (int)cells[i].character << " " 
                   << (int)cells[i].foreground << " " 
                   << cells[i].intensity << " " 
                   << (int)cells[i].background << " " 
                   << cells[i].blink << endl;
        }
        bitFile.close();
        cout << "✓ Битовые поля записаны в bitfields.txt" << endl;
    }
}

// Демонстрация чтения структурированных данных
void readStructuredData(const string& filename) {
    ifstream inFile(filename);
    
    if (!inFile.is_open()) {
        cerr << "Ошибка: файл не найден" << endl;
        return;
    }
    
    cout << "\n=== Парсинг структурированных данных ===" << endl;
    
    string token;
    int number;
    double pi;
    
    // Пропускаем заголовок и описательные строки
    string dummy;
    getline(inFile, dummy); // первая строка
    getline(inFile, dummy); // вторая строка
    
    // Читаем с использованием operator>>
    inFile >> token >> token >> number;
    cout << "Извлечённое целое: " << number << endl;
    
    inFile >> token >> token >> token >> token >> token >> pi;
    cout << "Извлечённое double: " << pi << endl;
}

int main() {
    const string filename = "output.txt";
    
    // Этап 1: Запись базовых текстовых данных
    writeToFile(filename);
    
    // Этап 2: Чтение базовых данных
    readFromFile(filename);
    
    // Этап 3: Структурированное чтение базовых данных
    readStructuredData(filename);
    
    // Этап 4: Работа со структурами PERSON из struct.cpp
    writeStructuredPersons(filename);
    readStructuredPersons(filename);
    
    // Этап 5: Демонстрация битовых полей CELL
    demonstrateBitFields();
    
    /*
     * РАСШИРЕННЫЕ ЗАМЕЧАНИЯ ДЛЯ РАБОТЫ СО СТРУКТУРАМИ:
     * 
     * 1. ТЕКСТОВАЯ СЕРИАЛИЗАЦИЯ СТРУКТУР (как в этом примере):
     *    - Преимущества: читаемость, переносимость между системами
     *    - Недостатки: больше размер файла, медленнее парсинг
     *    - Идеально для: конфигурационных файлов, логов, обмена данными
     * 
     * 2. БИНАРНАЯ СЕРИАЛИЗАЦИЯ СТРУКТУР (см. 02_binary_io.cpp):
     *    - Преимущества: скорость, компактность, точность float/double
     *    - Недостатки: проблемы переносимости (endianness, alignment)
     *    - Идеально для: быстро меняющихся данных, игр, высокопроизводительных систем
     * 
     * 3. БИТОВЫЕ ПОЛЯ (структура CELL):
     *    - Экономят память путём упаковки нескольких логических полей в одно
     *    - Внимание: порядок битов зависит от архитектуры процессора
     *    - Используйте для: флагов, небольших счётчиков, кодирования состояний
     * 
     * 4. АЛЬТЕРНАТИВНЫЕ ПОДХОДЫ ДЛЯ СЕРИАЛИЗАЦИИ:
     *    - JSON/XML через библиотеки (nlohmann/json, pugixml)
     *    - Protobuf для межплатформенного обмена
     *    - Boost.Serialization для сложных иерархий объектов
     * 
     * 5. БЕЗОПАСНОСТЬ: всегда проверяйте корректность данных при чтении,
     *    особенно при работе с массивами символов (buffer overflow protection)
     */
    
    return 0;
}

