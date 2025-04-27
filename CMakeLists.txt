

if(NOT DEFINED ENV{SERVER_FILES})
    message("--------------------------------------------------")
    message(" ОШИБКА: Системная переменная окружения SERVER_ROOT не задана")
    message("   1. Закройте текущее окно терминала/IDE")
    message("   2. Задайте переменную: <setx SERVER_FILES \"${CMAKE_CURRENT_SOURCE_DIR}/files\" /M>")
    message("   3. Перезапустите окно терминала/IDE и повторите сборку")
    message("--------------------------------------------------")
    message(FATAL_ERROR "SERVER_FILES not set. Please set environment variable first")
endif()


cmake_minimum_required(VERSION 3.10)

project(ServerProject 
    VERSION 1.0 
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Установка переменной среды
# set(ENV{SERVER_ROOT} "${CMAKE_CURRENT_SOURCE_DIR}")

# Пути к OpenSSL
set(OPENSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes/OpenSSL-Win64")
set(OPENSSL_INCLUDE "${OPENSSL_DIR}/include")
set(OPENSSL_LIBS "${OPENSSL_DIR}/lib")

# Путь к SQLite3 (заголовочный файл и DLL)
set(SQLITE3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")  # Путь к sqlite3.h
set(SQLITE3_DLL "${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite3.dll")  # Путь к DLL

# Поиск SQLite3 
find_package(SQLite3 REQUIRED)

add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Подключение заголовочных файлов
target_include_directories(server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${SQLITE3_INCLUDE_DIR}
    ${OPENSSL_INCLUDE}
)

# Линковка библиотек
target_link_libraries(server PRIVATE 
    ws2_32 
    ssl 
    crypto
    sqlite3  # Если используется find_package(SQLite3)
)

# Копирование sqlite3.dll в выходную директорию
add_custom_command(
    TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${SQLITE3_DLL}
        $<TARGET_FILE_DIR:server>
)

