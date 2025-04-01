#include "http/server.hpp"
#include "grpc/client.hpp"
#include "common/env_validator.hpp"
#include <dotenv.h>
#include <iostream>
#include <signal.h>

std::unique_ptr<HttpServer> http_server;
std::unique_ptr<GrpcClient> grpc_client;

void signal_handler(int signum) {
    std::cout << "Получен сигнал завершения. Останавливаем сервер..." << std::endl;
    if (http_server) {
        http_server->stop();
    }
    exit(signum);
}

int main() {
    try {
        dotenv::init();

        try {
            EnvValidator::validate();
        } catch (const EnvValidationError& e) {
            std::cerr << "Ошибка валидации конфигурации: " << e.what() << std::endl;
            return 1;
        }

        const std::string http_host = std::getenv("HTTP_HOST") ?: "0.0.0.0";
        const uint16_t http_port = std::stoi(std::getenv("HTTP_PORT") ?: "8080");
        const std::string grpc_address = std::getenv("GRPC_ADDRESS") ?: "localhost:50051";

        grpc_client = std::make_unique<GrpcClient>(grpc_address);

        http_server = std::make_unique<HttpServer>(http_host, http_port);

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);

        http_server->add_route("/<path>", crow::HTTPMethod::GET, 
            [](const crow::request& req) {
                return grpc_client->forward_request(req);
            });

        http_server->add_route("/<path>", crow::HTTPMethod::POST,
            [](const crow::request& req) {
                return grpc_client->forward_request(req);
            });

        http_server->add_route("/<path>", crow::HTTPMethod::PUT,
            [](const crow::request& req) {
                return grpc_client->forward_request(req);
            });

        http_server->add_route("/<path>", crow::HTTPMethod::DELETE,
            [](const crow::request& req) {
                return grpc_client->forward_request(req);
            });

        std::cout << "Запуск HTTP сервера на " << http_host << ":" << http_port << std::endl;
        std::cout << "Подключение к gRPC серверу на " << grpc_address << std::endl;
        
        http_server->start();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 