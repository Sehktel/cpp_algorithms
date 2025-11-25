# Простой Makefile для Ping-Pong демонстрации
# Только для Linux

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET_CLIENT = simple_client
TARGET_SERVER = simple_server

# Клиент
$(TARGET_CLIENT): simple_client.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_CLIENT) simple_client.cpp

# Сервер  
$(TARGET_SERVER): simple_server.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_SERVER) simple_server.cpp

# Сборка всех программ
all: $(TARGET_CLIENT) $(TARGET_SERVER)

# Очистка
clean:
	rm -f $(TARGET_CLIENT) $(TARGET_SERVER)

# Запуск сервера (в фоне)
run-server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

# Запуск клиента
run-client: $(TARGET_CLIENT)
	./$(TARGET_CLIENT)

.PHONY: all clean run-server run-client
