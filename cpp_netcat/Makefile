# ⚠️ LEGACY: Этот файл мигрирован в 02_networking/08_tcp_client_server/netcat/Makefile
# Этот файл сохранен для обратной совместимости и будет удален после завершения миграции.
# Используйте новый путь: 02_networking/08_tcp_client_server/netcat/Makefile

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Цели
all: netcat_server netcat_client

# Сервер
netcat_server: netcat_server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Клиент
netcat_client: netcat_client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Очистка скомпилированных файлов
clean:
	rm -f netcat_server netcat_client

.PHONY: all clean 