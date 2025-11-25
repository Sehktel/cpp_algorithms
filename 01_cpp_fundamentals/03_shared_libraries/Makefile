# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC

# Директория для установки библиотеки
LIBDIR = /usr/local/lib

# Имя библиотеки
LIBNAME = libmy_math.so

# Цели
all: $(LIBNAME) calc

# Создание shared object библиотеки
$(LIBNAME): my_math.o
	$(CXX) $(CXXFLAGS) -shared -o $(LIBNAME) my_math.o

# Компиляция объектного файла с позиционно-независимым кодом
my_math.o: my_math.cpp my_math.h
	$(CXX) $(CXXFLAGS) -c my_math.cpp -o my_math.o

# Сборка калькулятора со shared библиотекой
calc.o: calc.cpp my_math.h
	$(CXX) $(CXXFLAGS) -c calc.cpp -o calc.o

# Линковка калькулятора с shared библиотекой
calc: calc.o $(LIBNAME)
	$(CXX) $(CXXFLAGS) calc.o -L. -lmy_math -o calc

# Установка shared библиотеки в систему
install:
	mkdir -p $(LIBDIR)
	cp $(LIBNAME) $(LIBDIR)
	ldconfig

# Удаление установленной библиотеки
uninstall:
	rm -f $(LIBDIR)/$(LIBNAME)
	ldconfig

# Очистка скомпилированных файлов
clean:
	rm -f *.o $(LIBNAME) calc

# Фальшивые цели
.PHONY: all clean install uninstall 