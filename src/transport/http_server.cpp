#include "http_server.hpp"
#include <iostream>

HttpServer::HttpServer(const std::string& host, uint16_t port)
    : app_(std::make_unique<crow::SimpleApp>())
    , host_(host)
    , port_(port) {}

void HttpServer::start() {
    if (running_) {
        return;
    }

    running_ = true;
    std::cout << "Starting HTTP server on " << host_ << ":" << port_ << std::endl;
    
    app_->port(port_).multithreaded().run();
}

void HttpServer::stop() {
    if (!running_) {
        return;
    }

    running_ = false;
    app_->stop();
}

void HttpServer::add_route(const std::string& path, crow::HTTPMethod method, RequestHandler handler) {
    app_->route_dynamic(path)
        .methods(method)
        ([handler](const crow::request& req) {
            return handler(req);
        });
} 