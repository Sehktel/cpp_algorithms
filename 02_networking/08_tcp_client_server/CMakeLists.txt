cmake_minimum_required(VERSION 3.10)
project(TCPClientServer)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги компиляции
if(MSVC)
    add_compile_options(/W4 /WX- /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Простые примеры (simple/)
add_executable(simple_server simple/simple_server.cpp)
add_executable(simple_client simple/simple_client.cpp)

# Продвинутые примеры (advanced/)
add_executable(advanced_server advanced/server.cpp)
add_executable(advanced_client advanced/client.cpp)

# Ping-Pong приложение (ping_pong/)
add_executable(ping_pong_server ping_pong/ping_pong_server.cpp)
add_executable(ping_pong_client ping_pong/ping_pong_client.cpp)

# Netcat (netcat/)
add_executable(netcat_server netcat/netcat_server.cpp)
add_executable(netcat_client netcat/netcat_client.cpp)

# Линковка библиотек для Windows
if(WIN32)
    target_link_libraries(simple_server ws2_32)
    target_link_libraries(simple_client ws2_32)
    target_link_libraries(advanced_server ws2_32)
    target_link_libraries(advanced_client ws2_32)
    target_link_libraries(ping_pong_server ws2_32)
    target_link_libraries(ping_pong_client ws2_32)
    target_link_libraries(netcat_server ws2_32)
    target_link_libraries(netcat_client ws2_32)
endif()

# Линковка библиотек для Linux/macOS
if(UNIX)
    target_link_libraries(simple_server pthread)
    target_link_libraries(simple_client pthread)
    target_link_libraries(advanced_server pthread)
    target_link_libraries(advanced_client pthread)
    target_link_libraries(ping_pong_server pthread)
    target_link_libraries(ping_pong_client pthread)
    target_link_libraries(netcat_server pthread)
    target_link_libraries(netcat_client pthread)
endif()

# Устанавливаем выходную директорию
set_target_properties(
    simple_server
    simple_client
    advanced_server
    advanced_client
    ping_pong_server
    ping_pong_client
    netcat_server
    netcat_client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "===========================================")
message(STATUS "TCP Client-Server Configuration")
message(STATUS "===========================================")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "===========================================")
message(STATUS "Available targets:")
message(STATUS "  - simple_server / simple_client")
message(STATUS "  - advanced_server / advanced_client")
message(STATUS "  - ping_pong_server / ping_pong_client")
message(STATUS "  - netcat_server / netcat_client")
message(STATUS "===========================================")

