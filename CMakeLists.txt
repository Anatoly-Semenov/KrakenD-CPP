cmake_minimum_required(VERSION 3.10)
project(krakend_proxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Генерируем protobuf файлы
add_proto_files(
    TARGET krakend_proto
    PROTOS proto/krakend.proto
    PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto
)

# Добавляем Crow (предполагая, что он установлен в системе)
find_package(Crow REQUIRED)

# Добавляем dotenv библиотеку
add_library(dotenv INTERFACE)
target_include_directories(dotenv INTERFACE
    ${CMAKE_SOURCE_DIR}/third_party/dotenv
)

# Основной исполняемый файл
add_executable(krakend_proxy
    src/main.cpp
        src/transport/http_server.cpp
        src/transport/grpc_client.cpp
)

# Включаем директории
target_include_directories(krakend_proxy PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/proto
)

# Линкуем библиотеки
target_link_libraries(krakend_proxy PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    Boost::system
    Boost::thread
    dotenv
    krakend_proto
    gRPC::grpc++
    Crow::Crow
) 