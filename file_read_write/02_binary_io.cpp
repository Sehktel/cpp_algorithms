// ⚠️ LEGACY: Этот файл мигрирован в 01_cpp_fundamentals/07_file_io/02_binary_io.cpp
// Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
// Используйте новый путь: 01_cpp_fundamentals/07_file_io/02_binary_io.cpp

/*
 * Лабораторная работа #2: Бинарный ввод/вывод
 * 
 * Тема: Работа с бинарными данными, сериализация структур
 * 
 * Ключевое различие:
 * - Текстовые файлы: данные преобразуются в символы (форматированный I/O)
 * - Бинарные файлы: побайтовая копия памяти (неформатированный I/O)
 * 
 * Преимущества бинарных файлов:
 * - Компактность (int 42 занимает 4 байта вместо 2 символов)
 * - Скорость (нет преобразования типов)
 * - Точность (числа с плавающей точкой без потери точности)
 * 
 * Недостатки:
 * - Не читаемы человеком
 * - Проблемы с переносимостью (endianness, padding)
 */

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Структура для демонстрации сериализации
struct Student {
    char name[50];
    int id;
    double gpa;
    
    // Конструктор для удобства инициализации
    Student(const char* n = "", int i = 0, double g = 0.0) 
        : id(i), gpa(g) {
        strncpy(name, n, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0'; // Гарантируем null-termination
    }
    
    void display() const {
        cout << "Имя: " << name 
             << ", ID: " << id 
             << ", Средний балл: " << gpa << endl;
    }
};

// Запись структуры в бинарный файл
void writeBinaryData(const string& filename) {
    /*
     * ios::binary - критически важный флаг!
     * Без него на Windows происходит конвертация \n в \r\n
     * Это приводит к порче бинарных данных
     */
    ofstream outFile(filename, ios::binary);
    
    if (!outFile.is_open()) {
        cerr << "Ошибка открытия файла для бинарной записи" << endl;
        return;
    }
    
    // Создаём массив студентов
    Student students[] = {
        Student("Иванов Иван", 101, 4.5),
        Student("Петрова Мария", 102, 4.8),
        Student("Сидоров Пётр", 103, 4.2)
    };
    
    int count = sizeof(students) / sizeof(students[0]);
    
    // Сначала записываем количество записей
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    // Затем записываем массив структур
    /*
     * write() принимает:
     * - const char* - указатель на байты
     * - streamsize - количество байтов
     * 
     * reinterpret_cast<const char*> - интерпретируем память объекта как массив байтов
     * Это type punning - мощная, но опасная техника
     */
    outFile.write(reinterpret_cast<const char*>(students), sizeof(students));
    
    cout << "✓ Записано " << count << " записей в бинарный файл" << endl;
}

// Чтение структуры из бинарного файла
void readBinaryData(const string& filename) {
    ifstream inFile(filename, ios::binary);
    
    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла для бинарного чтения" << endl;
        return;
    }
    
    // Читаем количество записей
    int count;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    cout << "\n=== Чтение из бинарного файла ===" << endl;
    cout << "Найдено записей: " << count << endl << endl;
    
    // Динамическое выделение памяти для массива
    Student* students = new Student[count];
    
    // Читаем все записи
    inFile.read(reinterpret_cast<char*>(students), sizeof(Student) * count);
    
    // Проверка успешности чтения
    if (inFile.gcount() != sizeof(Student) * count) {
        cerr << "Предупреждение: прочитано байт: " << inFile.gcount() 
             << ", ожидалось: " << sizeof(Student) * count << endl;
    }
    
    // Отображаем прочитанные данные
    for (int i = 0; i < count; i++) {
        students[i].display();
    }
    
    // Освобождаем память
    delete[] students;
    
    /*
     * КРИТИЧЕСКОЕ ЗАМЕЧАНИЕ:
     * Такая сериализация работает только для POD (Plain Old Data) типов!
     * 
     * НЕ используйте это для структур с:
     * - Указателями (сохранится адрес, а не данные)
     * - std::string (внутренняя структура сложнее)
     * - Виртуальными функциями (vtable pointer)
     * 
     * Для сложных объектов используйте:
     * - Протокол сериализации (Protocol Buffers, Cap'n Proto)
     * - JSON/XML для переносимости
     * - Boost.Serialization
     */
}

// Демонстрация работы с произвольным доступом
void randomAccessDemo(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }
    
    cout << "\n=== Демонстрация произвольного доступа ===" << endl;
    
    // Читаем количество записей
    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Переходим ко второй записи (индекс 1)
    /*
     * seekg - установить позицию get pointer (для чтения)
     * seekp - установить позицию put pointer (для записи)
     * 
     * Параметры:
     * - offset: смещение в байтах
     * - direction: ios::beg (начало), ios::cur (текущая), ios::end (конец)
     */
    streampos recordPosition = sizeof(int) + sizeof(Student) * 1;
    file.seekg(recordPosition);
    
    Student student;
    file.read(reinterpret_cast<char*>(&student), sizeof(Student));
    
    cout << "Прочитана вторая запись: ";
    student.display();
    
    // Модифицируем GPA
    student.gpa = 5.0;
    
    // Возвращаемся назад и перезаписываем
    file.seekp(recordPosition);
    file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
    
    cout << "✓ Запись обновлена" << endl;
}

int main() {
    const string filename = "students.dat";
    
    // Этап 1: Запись бинарных данных
    writeBinaryData(filename);
    
    // Этап 2: Чтение бинарных данных
    readBinaryData(filename);
    
    // Этап 3: Произвольный доступ и модификация
    randomAccessDemo(filename);
    
    // Этап 4: Проверка изменений
    cout << "\n=== После модификации ===" << endl;
    readBinaryData(filename);
    
    /*
     * ЗАДАНИЕ ДЛЯ САМОСТОЯТЕЛЬНОЙ РАБОТЫ:
     * 
     * 1. Добавьте функцию для поиска студента по ID
     * 2. Реализуйте удаление записи (hint: используйте временный файл)
     * 3. Добавьте индексный файл для быстрого поиска (B-tree концепция)
     * 4. * Реализуйте версионирование формата файла для обратной совместимости
     */
    
    return 0;
}


