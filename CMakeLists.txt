
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


# Пути к SSL
set(OPENSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes/OpenSSL-Win64")
set(OPENSSL_INCLUDE "${OPENSSL_DIR}/include")
set(OPENSSL_LIBS "${OPENSSL_DIR}/lib")
# Проверка наличия OpenSSL
if(NOT EXISTS "${OPENSSL_INCLUDE}/openssl/ssl.h")
    message(FATAL_ERROR "OpenSSL headers not found in ${OPENSSL_INCLUDE}")
endif()


# Пути к SQLite3
set(SQLITE3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")  # sqlite3.h лежит прямо в includes/
set(SQLITE3_DLL "${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite3.dll")
# Проверка наличия SQLite3
if(NOT EXISTS "${SQLITE3_INCLUDE_DIR}/sqlite3.h")
    message(FATAL_ERROR "sqlite3.h not found in ${SQLITE3_INCLUDE_DIR}")
endif()


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
    sqlite3
)

# Копирование sqlite3.dll в выходную директорию
add_custom_command(
    TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${SQLITE3_DLL}
        $<TARGET_FILE_DIR:server>
)