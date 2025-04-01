# KrakenD-CPP

Этот проект реализует HTTP прокси-сервер, который принимает HTTP запросы и пересылает их через gRPC в KrakenD API Gateway. Сервер обеспечивает надежную и эффективную маршрутизацию запросов между HTTP клиентами и KrakenD.

## Основные возможности

- Прием HTTP запросов (GET, POST, PUT, DELETE)
- Пересылка запросов через gRPC в KrakenD
- Валидация входящих и исходящих данных
- Многопоточная обработка запросов
- Graceful shutdown при получении сигналов завершения

## Требования

- C++17
- CMake 3.10 или выше
- gRPC
- Protocol Buffers
- Crow (C++ web framework)
- Boost
- nlohmann/json
- libcurl

## Установка зависимостей

### macOS
```bash
brew install grpc protobuf boost nlohmann-json curl
```

### Ubuntu/Debian
```bash
sudo apt-get install libgrpc++-dev protobuf-compiler-grpc libboost-all-dev nlohmann-json3-dev libcurl4-openssl-dev
```

## Сборка

```bash
mkdir build
cd build
cmake ..
make
```

## Конфигурация

Создайте файл `.env` в корневой директории проекта со следующими параметрами:

```env
# HTTP Server Configuration
HTTP_HOST=0.0.0.0
HTTP_PORT=8080

# gRPC Configuration
GRPC_ADDRESS=localhost:50051

# KrakenD Configuration
KRAKEND_HOST=localhost
KRAKEND_PORT=8081
KRAKEND_TIMEOUT=30
KRAKEND_MAX_RETRIES=3
KRAKEND_RETRY_DELAY=1
KRAKEND_SSL_VERIFY=true
KRAKEND_SSL_CERT_PATH=/path/to/cert.pem
KRAKEND_SSL_KEY_PATH=/path/to/key.pem
KRAKEND_DEBUG=false
KRAKEND_LOG_LEVEL=info
```

### Описание параметров конфигурации

#### HTTP Server
- `HTTP_HOST` - хост для HTTP сервера (по умолчанию: 0.0.0.0)
- `HTTP_PORT` - порт для HTTP сервера (по умолчанию: 8080)

#### gRPC
- `GRPC_ADDRESS` - адрес gRPC сервера (по умолчанию: localhost:50051)

#### KrakenD
- `KRAKEND_HOST` - хост KrakenD сервера (по умолчанию: localhost)
- `KRAKEND_PORT` - порт KrakenD сервера (по умолчанию: 8081)
- `KRAKEND_TIMEOUT` - таймаут запросов в секундах (по умолчанию: 30)
- `KRAKEND_MAX_RETRIES` - максимальное количество попыток повторной отправки запроса (по умолчанию: 3)
- `KRAKEND_RETRY_DELAY` - задержка между попытками в секундах (по умолчанию: 1)
- `KRAKEND_SSL_VERIFY` - проверка SSL сертификата (по умолчанию: true)
- `KRAKEND_SSL_CERT_PATH` - путь к SSL сертификату
- `KRAKEND_SSL_KEY_PATH` - путь к SSL ключу
- `KRAKEND_DEBUG` - режим отладки (по умолчанию: false)
- `KRAKEND_LOG_LEVEL` - уровень логирования (по умолчанию: info)

## Запуск

```bash
./krakend_proxy
```

## Структура проекта

```
.
├── CMakeLists.txt          # Конфигурация сборки проекта
├── README.md              # Документация проекта
├── .env                   # Конфигурация переменных окружения
├── include/               # Заголовочные файлы
│   ├── http/             # HTTP компоненты
│   │   ├── server.hpp    # HTTP сервер на базе Crow
│   │   └── client.hpp    # HTTP клиент
│   ├── grpc/             # gRPC компоненты
│   │   ├── client.hpp    # gRPC клиент для KrakenD
│   │   └── server.hpp    # gRPC сервер
│   └── validator/        # Валидаторы
│       ├── env_validator.hpp    # Валидатор .env переменных
│       └── validator.hpp        # Валидатор входящих и исходящих данных
├── src/                   # Исходный код
│   ├── main.cpp          # Точка входа и конфигурация приложения
│   ├── http/             # Реализация HTTP компонентов
│   │   ├── server.cpp    # Реализация HTTP сервера
│   │   └── client.cpp    # Реализация HTTP клиента
│   └── grpc/             # Реализация gRPC компонентов
│       ├── client.cpp    # Реализация gRPC клиента
│       └── server.cpp    # Реализация gRPC сервера
└── proto/                 # Protocol Buffers определения
    └── krakend.proto     # Определение gRPC сервиса
```

### Добавление новых маршрутов

Для добавления нового маршрута в `main.cpp`:

```cpp
http_server->add_route("/new-path", crow::HTTPMethod::GET,
    [](const crow::request& req) {
        return grpc_client->forward_request(req);
    });
```
