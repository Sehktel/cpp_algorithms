# Внутренняя архитектура линковщика

## 1. Базовая модель линковки

### Абстракция процесса линковки
```
+-------------------+
| Исходные файлы    |
|  - .o             |
|  - .a             |
+-------------------+
        ↓
+-------------------+
| Линковщик         |
|  1. Парсинг       |
|  2. Резолвинг     |
|  3. Релокация     |
+-------------------+
        ↓
+-------------------+
| Исполняемый файл  |
|  или библиотека   |
+-------------------+
```

## 2. Внутренняя структура линковщика

### Ключевые структуры данных

```cpp
// Представление символа в линковщике
struct Symbol {
    enum Type {
        UNDEFINED,
        DEFINED,
        WEAK,
        GLOBAL
    };

    std::string name;         // Имя символа
    Type type;                // Тип символа
    uintptr_t address;        // Виртуальный адрес
    size_t size;              // Размер символа
    ElfSection* section;      // Секция, где определен символ
};

// Структура для релокационной записи
struct Relocation {
    uintptr_t offset;         // Смещение в секции
    Symbol* symbol;           // Связанный символ
    RelocationType type;      // Тип релокации
    int64_t addend;           // Дополнительное значение
};

// Представление объектного файла
struct ObjectFile {
    std::string filename;
    std::vector<Symbol> symbols;
    std::vector<Relocation> relocations;
    std::vector<ElfSection> sections;
};
```

## 3. Алгоритм линковки

### Псевдокод процесса линковки

```cpp
class Linker {
public:
    void link(std::vector<ObjectFile>& objects) {
        // 1. Сбор всех символов
        collectSymbols(objects);
        
        // 2. Разрешение символических ссылок
        resolveSymbols();
        
        // 3. Выполнение релокаций
        performRelocations();
        
        // 4. Генерация выходного файла
        generateOutput();
    }

private:
    void collectSymbols(std::vector<ObjectFile>& objects) {
        for (auto& obj : objects) {
            for (auto& symbol : obj.symbols) {
                // Проверка конфликтов символов
                if (symbolTable.count(symbol.name)) {
                    resolveSymbolConflict(symbol);
                }
                symbolTable[symbol.name] = &symbol;
            }
        }
    }

    void resolveSymbols() {
        for (auto& [name, symbol] : symbolTable) {
            if (symbol->type == Symbol::UNDEFINED) {
                // Поиск определения символа
                auto definition = findSymbolDefinition(name);
                if (definition) {
                    symbol->address = definition->address;
                } else {
                    // Ошибка неразрешенного символа
                    throw LinkError("Undefined symbol: " + name);
                }
            }
        }
    }

    void performRelocations() {
        for (auto& obj : objectFiles) {
            for (auto& reloc : obj.relocations) {
                // Применение релокации
                applyRelocation(reloc);
            }
        }
    }
};
```

## 4. Типы релокаций

### Основные виды релокаций

```cpp
enum RelocationType {
    R_X86_64_32,       // 32-битная абсолютная релокация
    R_X86_64_64,       // 64-битная абсолютная релокация
    R_X86_64_PC32,     // 32-битная относительная релокация
    R_X86_64_GOT32,    // Релокация через GOT
    R_X86_64_PLT32     // Релокация через PLT
};

void applyRelocation(Relocation& reloc) {
    switch (reloc.type) {
        case R_X86_64_32:
            // Простая 32-битная абсолютная релокация
            *reinterpret_cast<uint32_t*>(reloc.offset) = 
                reloc.symbol->address + reloc.addend;
            break;
        
        case R_X86_64_PC32:
            // Относительная релокация
            *reinterpret_cast<int32_t*>(reloc.offset) = 
                reloc.symbol->address - (reloc.offset + 4) + reloc.addend;
            break;
        
        // Другие типы релокаций...
    }
}
```

## 5. Сложные случаи линковки

### Обработка слабых символов

```cpp
class WeakSymbolResolver {
public:
    Symbol* resolveWeakSymbol(const std::string& name) {
        // Приоритет: 
        // 1. Сильный глобальный символ
        // 2. Слабый глобальный символ
        // 3. Локальный символ
        
        Symbol* strongGlobal = findStrongGlobalSymbol(name);
        if (strongGlobal) return strongGlobal;
        
        Symbol* weakGlobal = findWeakGlobalSymbol(name);
        if (weakGlobal) return weakGlobal;
        
        return findLocalSymbol(name);
    }
};
```

## 6. Диагностика и отладка

```cpp
class LinkerDebugger {
public:
    void dumpSymbolTable(const SymbolTable& table) {
        for (const auto& [name, symbol] : table) {
            std::cout << "Symbol: " << name 
                      << ", Address: " << symbol->address 
                      << ", Type: " << getSymbolTypeName(symbol->type) 
                      << std::endl;
        }
    }

    void validateRelocations(const std::vector<Relocation>& relocs) {
        for (const auto& reloc : relocs) {
            if (!isValidRelocation(reloc)) {
                reportRelocationError(reloc);
            }
        }
    }
};
```

## Заключение

Линковщик - это сложная система трансформации объектных файлов, требующая глубокого понимания архитектуры процессора, форматов файлов и механизмов выполнения программ.

---

*© Техническое руководство по разработке линковщика* 