/*
 * Лабораторная работа #8: Примеры использования stringstream
 * 
 * Тема: Работа с stringstream, istringstream, ostringstream
 * 
 * Концепции:
 * - stringstream - поток для чтения и записи строк
 * - istringstream - поток только для чтения из строки
 * - ostringstream - поток только для записи в строку
 * - Парсинг строк с различными типами данных
 * - Форматирование вывода в строку
 * - Извлечение токенов из строки
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

// Пример 1: Базовое использование stringstream для конвертации типов
void basicStringStreamExample() {
    cout << "\n=== Пример 1: Базовые конвертации stringstream ===" << endl;
    
    // stringstream может работать как istringstream (чтение) и ostringstream (запись)
    stringstream ss;
    
    // Записываем различные типы данных в поток
    int number = 42;
    double pi = 3.14159;
    string text = "Привет";
    
    ss << "Число: " << number << ", Pi: " << pi << ", Текст: " << text;
    
    // Получаем полученную строку
    string result = ss.str();
    cout << "Результат записи: " << result << endl;
    
    // Теперь читаем данные обратно из той же строки
    stringstream ss2(result);
    string word;
    
    cout << "Разбор строки по словам:" << endl;
    while (ss2 >> word) {
        cout << "  Слово: '" << word << "'" << endl;
    }
}

// Пример 2: Парсинг строки с разделителями
void parsingExample() {
    cout << "\n=== Пример 2: Парсинг CSV-подобных данных ===" << endl;
    
    // Данные в формате: имя,возраст,зарплата
    string csvLine = "Иванов,25,50000.50";
    
    // Используем istringstream для чтения
    istringstream iss(csvLine);
    string name, ageStr, salaryStr;
    
    // Извлекаем данные до запятой
    getline(iss, name, ',');      // Читаем до запятой
    getline(iss, ageStr, ',');    // Читаем до следующей запятой
    getline(iss, salaryStr);      // Читаем до конца строки
    
    // Конвертируем строки в соответствующие типы
    int age = stoi(ageStr);
    double salary = stod(salaryStr);
    
    cout << "Парсинг CSV:" << endl;
    cout << "  Имя: " << name << endl;
    cout << "  Возраст: " << age << " лет" << endl;
    cout << "  Зарплата: " << fixed << setprecision(2) << salary << " руб." << endl;
}

// Пример 3: Форматирование вывода в строку
void formattingExample() {
    cout << "\n=== Пример 3: Форматирование вывода ===" << endl;
    
    ostringstream oss;
    
    // Настраиваем форматирование
    oss << fixed << setprecision(2);
    oss << left << setw(15) << "Продукт" 
        << right << setw(10) << "Цена" 
        << right << setw(8) << "К-во" 
        << right << setw(12) << "Сумма" << endl;
    
    // Рисуем разделительную линию
    oss << string(50, '-') << endl;
    
    // Добавляем данные товаров
    vector<pair<string, double>> products = {
        {"Хлеб", 25.50},
        {"Молоко", 45.00},
        {"Сыр", 120.75}
    };
    
    for (const auto& product : products) {
        int quantity = 3; // пример
        double total = product.second * quantity;
        
        oss << left << setw(15) << product.first
            << right << setw(10) << product.second
            << right << setw(8) << quantity
            << right << setw(12) << total << endl;
    }
    
    string table = oss.str();
    cout << "Сформированная таблица:" << endl;
    cout << table;
}

// Пример 4: Извлечение чисел из смешанной строки
void extractNumbersExample() {
    cout << "\n=== Пример 4: Извлечение чисел из строки ===" << endl;
    
    string mixedText = "У меня есть 5 яблок и 3.14 килограмма апельсинов, также 100 рублей";
    
    istringstream iss(mixedText);
    string word;
    
    cout << "Извлечение чисел из строки: '" << mixedText << "'" << endl;
    cout << "Найденные числа:" << endl;
    
    while (iss >> word) {
        // Пытаемся извлечь число из слова
        istringstream numberStream(word);
        double number;
        
        if (numberStream >> number) {
            // Проверяем, что после числа нет дополнительных символов
            char remaining;
            if (!(numberStream >> remaining)) {
                cout << "  Число: " << number << endl;
            }
        }
    }
}

// Пример 5: Создание собственного "калькулятора выражений"
void simpleCalculatorExample() {
    cout << "\n=== Пример 5: Простой калькулятор выражений ===" << endl;
    
    // Выражение вида "a + b" или "a - b"
    string expression = "15 + 27";
    
    istringstream iss(expression);
    double a, b;
    string operation;
    
    // Парсим выражение
    if (iss >> a >> operation >> b) {
        double result = 0;
        
        // Выполняем операцию
        if (operation == "+") {
            result = a + b;
        } else if (operation == "-") {
            result = a - b;
        } else if (operation == "*") {
            result = a * b;
        } else if (operation == "/") {
            if (b != 0) {
                result = a / b;
            } else {
                cout << "Ошибка: деление на ноль!" << endl;
                return;
            }
        } else {
            cout << "Неподдерживаемая операция: " << operation << endl;
            return;
        }
        
        // Форматируем результат
        ostringstream resultStream;
        resultStream << fixed << setprecision(2);
        resultStream << a << " " << operation << " " << b << " = " << result;
        
        cout << "Результат: " << resultStream.str() << endl;
    } else {
        cout << "Не удалось разобрать выражение: " << expression << endl;
    }
}

// Пример 6: Работа с многострочным текстом
void multilineTextExample() {
    cout << "\n=== Пример 6: Обработка многострочного текста ===" endl;
    
    // Многострочный текст с данными
    string multilineData = R"(
        Студент: Петров И.И.
        Оценки: 5 4 5 3 4
        Средний балл: 4.2
    )";
    
    istringstream iss(multilineData);
    string line;
    vector<int> grades;
    
    cout << "Обработка многострочных данных:" << endl;
    
    while (getline(iss, line)) {
        // Убираем лишние пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;
        
        cout << "Строка: '" << line << "'" << endl;
        
        // Если строка содержит оценку, извлекаем её
        if (line.find("Оценки:") != string::npos) {
            istringstream gradeStream(line);
            string word;
            
            // Пропускаем слово "Оценки:"
            gradeStream >> word;
            
            // Извлекаем все числа
            int grade;
            while (gradeStream >> grade) {
                grades.push_back(grade);
            }
        }
    }
    
    if (!grades.empty()) {
        cout << "Извлечённые оценки: ";
        for (int grade : grades) {
            cout << grade << " ";
        }
        cout << endl;
    }
}

// Пример 7: Тестирование различных функций stringstream
void advancedStringStreamTest() {
    cout << "\n=== Пример 7: Продвинутые возможности ===" << endl;
    
    // Демонстрация методов stringstream
    stringstream ss;
    
    // Проверяем, пуст ли поток
    cout << "Поток пуст? " << (ss.str().empty() ? "Да" : "Нет") << endl;
    
    // Записываем данные
    ss << "Тест " << 123 << " " << true;
    cout << "После записи: '" << ss.str() << "'" << endl;
    
    // Очищаем поток
    ss.str("");  // Очищаем содержимое
    ss.clear();  // Сбрасываем флаги ошибок
    
    cout << "После очистки: '" << ss.str() << "'" << endl;
    
    // Проверяем состояние потока
    ss << "Новые данные";
    cout << "Состояние потока после записи:" << endl;
    cout << "  good(): " << ss.good() << endl;
    cout << "  eof(): " << ss.eof() << endl;
    cout << "  fail(): " << ss.fail() << endl;
    cout << "  bad(): " << ss.bad() << endl;
}

int main() {
    cout << "Добро пожаловать в демонстрацию stringstream!" << endl;
    cout << "Этот файл содержит учебные примеры использования потоков строк в C++." << endl;
    
    // Запускаем все примеры
    basicStringStreamExample();
    parsingExample();
    formattingExample();
    extractNumbersExample();
    simpleCalculatorExample();
    multilineTextExample();
    advancedStringStreamTest();
    
    cout << "\n=== Заключение ===" << endl;
    cout << "Основные применения stringstream:" << endl;
    cout << "1. Конвертация между строками и числовыми типами" << endl;
    cout << "2. Парсинг структурированных данных (CSV, конфигурации)" << endl;
    cout << "3. Форматирование вывода без непосредственного вывода на консоль" << endl;
    cout << "4. Извлечение токенов из строки" << endl;
    cout << "5. Создание простых парсеров и интерпретаторов" << endl;
    
    cout << "\nПомните: stringstream создаёт временный буфер в памяти," << endl;
    cout << "используйте его для небольших объёмов данных или" << endl;
    cout << "когда нужна гибкость в форматировании." << endl;
    
    return 0;
}

/*
 * ТЕОРЕТИЧЕСКИЕ ЗАМЕЧАНИЯ:
 * 
 * 1. STRINGSTREAM vs ДРУГИЕ ПОДХОДЫ:
 *    - stringstream удобен для парсинга и форматирования
 *    - Для простых случаев stoi(), stod() могут быть быстрее
 *    - Для сложного парсинга рассмотрите регулярные выражения
 * 
 * 2. ПРОИЗВОДИТЕЛЬНОСТЬ:
 *    - stringstream создаёт динамическую память для буфера
 *    - Если нужно часто переиспользовать, создайте один объект и очищайте его
 *    - Для высокопроизводительного кода рассмотрите альтернативы
 * 
 * 3. БЕЗОПАСНОСТЬ:
 *    - Всегда проверяйте состояние потока после операций чтения
 *    - Используйте методы good(), fail(), eof(), bad()
 *    - При парсинге пользовательского ввода обязательно валидируйте данные
 * 
 * 4. АЛЬТЕРНАТИВЫ:
 *    - Для JSON/XML используйте специализированные библиотеки
 *    - Для высокопроизводительного парсинга рассмотрите boost::spirit
 *    - Для простых случаев подойдут функции из <algorithm>
 */

