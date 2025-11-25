# CMakeLists.txt для лабораторных работ по файловому I/O

cmake_minimum_required(VERSION 3.10)
project(FileIO_Labs CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Флаги компиляции для различных компиляторов
if(MSVC)
    # Visual Studio
    add_compile_options(/W4 /WX- /permissive-)
    # Оптимизация для Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    # GCC / Clang
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Оптимизация для Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Создаём исполняемые файлы для каждой лабораторной работы
add_executable(01_basic_text_io 01_basic_text_io.cpp)
add_executable(02_binary_io 02_binary_io.cpp)
add_executable(03_error_handling 03_error_handling.cpp)
add_executable(04_advanced_techniques 04_advanced_techniques.cpp)

# Пинг-понг сервер и клиент
add_executable(ping_pong_server ping_pong_server.cpp)
add_executable(ping_pong_client ping_pong_client.cpp)

# Настройка сетевых библиотек для Windows
if(WIN32)
    target_link_libraries(ping_pong_server ws2_32)
    target_link_libraries(ping_pong_client ws2_32)
endif()

# Устанавливаем выходную директорию
set_target_properties(
    01_basic_text_io
    02_binary_io
    03_error_handling
    04_advanced_techniques
    ping_pong_server
    ping_pong_client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Опциональная цель для запуска всех программ
add_custom_target(run_all
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running Lab 01: Basic Text I/O ==="
    COMMAND 01_basic_text_io
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running Lab 02: Binary I/O ==="
    COMMAND 02_binary_io
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running Lab 03: Error Handling ==="
    COMMAND 03_error_handling
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running Lab 04: Advanced Techniques ==="
    COMMAND 04_advanced_techniques
    DEPENDS 01_basic_text_io 02_binary_io 03_error_handling 04_advanced_techniques
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Цель для очистки сгенерированных файлов данных
add_custom_target(clean_data
    COMMAND ${CMAKE_COMMAND} -E remove -f
        output.txt
        students.dat
        test_data.txt
        config.ini
        formatted_output.txt
        perf_test1.txt
        perf_test2.txt
        perf_test3.txt
        perf_test4.txt
        data.csv
        large_file.txt
        test_format.txt
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "===========================================")
message(STATUS "File I/O Laboratory Works Configuration")
message(STATUS "===========================================")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "===========================================")
message(STATUS "Available targets:")
message(STATUS "  - 01_basic_text_io")
message(STATUS "  - 02_binary_io")
message(STATUS "  - 03_error_handling")
message(STATUS "  - 04_advanced_techniques")
message(STATUS "  - ping_pong_server (ping-pong server)")
message(STATUS "  - ping_pong_client (ping-pong client)")
message(STATUS "  - run_all (execute all labs)")
message(STATUS "  - clean_data (remove generated files)")
message(STATUS "===========================================")


