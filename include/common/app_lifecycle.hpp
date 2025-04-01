#pragma once

#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

class AppLifecycle {
public:
    static AppLifecycle& getInstance();
    
    void initialize();
    void shutdown();
    bool isShuttingDown() const;
    
    // Регистрация обработчиков завершения
    void addShutdownHandler(std::function<void()> handler);
    
private:
    AppLifecycle() = default;
    ~AppLifecycle() = default;
    AppLifecycle(const AppLifecycle&) = delete;
    AppLifecycle& operator=(const AppLifecycle&) = delete;

    std::atomic<bool> shutting_down_{false};
    std::mutex mutex_;
    std::condition_variable shutdown_cv_;
    std::vector<std::function<void()>> shutdown_handlers_;
}; 