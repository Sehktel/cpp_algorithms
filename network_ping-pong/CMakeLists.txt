cmake_minimum_required(VERSION 3.10)
project(NetworkPingPong)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Серверное приложение
add_executable(ping_pong_server 
    server/server.cpp
)

# Клиентское приложение
add_executable(ping_pong_client 
    client/client.cpp
)

# Линковка библиотек для Windows
if(WIN32)
    target_link_libraries(ping_pong_server ws2_32)
    target_link_libraries(ping_pong_client ws2_32)
endif()

# Линковка библиотек для Linux/macOS (pthread может понадобиться для расширений)
if(UNIX)
    target_link_libraries(ping_pong_server pthread)
    target_link_libraries(ping_pong_client pthread)
endif()

