#pragma once

#include <string>
#include <functional>
#include <memory>
#include <crow.h>

class HttpServer {
public:
    using RequestHandler = std::function<crow::response(const crow::request&)>;

    HttpServer(const std::string& host, uint16_t port);
    void start();
    void stop();
    
    // Регистрация обработчиков маршрутов
    void add_route(const std::string& path, crow::HTTPMethod method, RequestHandler handler);

private:
    std::unique_ptr<crow::SimpleApp> app_;
    std::string host_;
    uint16_t port_;
    bool running_ = false;
}; 