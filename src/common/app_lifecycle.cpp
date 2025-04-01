#include "common/app_lifecycle.hpp"
#include <iostream>

AppLifecycle& AppLifecycle::getInstance() {
    static AppLifecycle instance;
    return instance;
}

void AppLifecycle::initialize() {
    std::cout << "Инициализация приложения..." << std::endl;
}

void AppLifecycle::shutdown() {
    if (shutting_down_.exchange(true)) {
        return; // Уже выполняется shutdown
    }

    std::cout << "Начало graceful shutdown..." << std::endl;
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& handler : shutdown_handlers_) {
            try {
                handler();
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при выполнении shutdown handler: " << e.what() << std::endl;
            }
        }
    }

    std::cout << "Graceful shutdown завершен" << std::endl;
    shutdown_cv_.notify_all();
}

bool AppLifecycle::isShuttingDown() const {
    return shutting_down_;
}

void AppLifecycle::addShutdownHandler(std::function<void()> handler) {
    std::lock_guard<std::mutex> lock(mutex_);
    shutdown_handlers_.push_back(std::move(handler));
} 