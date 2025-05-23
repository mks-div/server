# Проверка переменной окружения SERVER_FILES
if(NOT DEFINED ENV{SERVER_FILES})
    message("--------------------------------------------------")
    message(" ОШИБКА: Системная переменная окружения SERVER_FILES не задана")
    message("   1. Закройте текущее окно терминала/IDE")
    message("   2. Задайте переменную: setx SERVER_FILES \"${CMAKE_CURRENT_SOURCE_DIR}/files\"")
    message("   3. Перезапустите терминал/IDE и повторите сборку")
    message("--------------------------------------------------")
    message(FATAL_ERROR "SERVER_FILES not set. Please set environment variable first")
endif()

# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Проект с поддержкой C и C++
project(ServerProject 
    VERSION 1.0 
    LANGUAGES C CXX
)

# Использовать C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги для статической линковки
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

# Пути к OpenSSL
set(OPENSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes/OpenSSL-Win64")
set(OPENSSL_INCLUDE "${OPENSSL_DIR}/include")
set(OPENSSL_LIBS "${OPENSSL_DIR}/lib")

# Проверка наличия заголовочных файлов OpenSSL
if(NOT EXISTS "${OPENSSL_INCLUDE}/openssl/ssl.h")
    message(FATAL_ERROR "OpenSSL headers not found in ${OPENSSL_INCLUDE}")
endif()

# Путь к SQLite3
set(SQLITE3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite3")
message(STATUS "Using SQLite3 from: ${SQLITE3_DIR}")

# Проверка наличия sqlite3.h и sqlite3.c
if(NOT EXISTS "${SQLITE3_DIR}/sqlite3.h" OR NOT EXISTS "${SQLITE3_DIR}/sqlite3.c")
    message(FATAL_ERROR "SQLite3 source files not found in ${SQLITE3_DIR}")
endif()

# Добавление SQLite как статической библиотеки
add_library(sqlite3 STATIC
    "${SQLITE3_DIR}/sqlite3.c"
)

# Явно указываем, что это C-библиотека
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
set_source_files_properties("${SQLITE3_DIR}/sqlite3.c" PROPERTIES LANGUAGE C)

# Подключение заголовочных файлов SQLite
target_include_directories(sqlite3 PUBLIC ${SQLITE3_DIR})

# Создание исполняемого файла
add_executable(server ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Поиск библиотек OpenSSL
find_library(SSL_LIBRARY ssl HINTS ${OPENSSL_LIBS})
find_library(CRYPTO_LIBRARY crypto HINTS ${OPENSSL_LIBS})

if(SSL_LIBRARY AND CRYPTO_LIBRARY)
    target_link_libraries(server PRIVATE 
        ws2_32
        ${SSL_LIBRARY}
        ${CRYPTO_LIBRARY}
        sqlite3
        pthread
        crypt32
        advapi32
        gdi32
        wsock32
    )
else()
    message(FATAL_ERROR "OpenSSL libraries not found in ${OPENSSL_LIBS}")
endif()

# Подключение заголовочных файлов проекта
target_include_directories(server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${SQLITE3_DIR}
    ${OPENSSL_INCLUDE}
)