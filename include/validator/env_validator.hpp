#pragma once

#include <string>
#include <stdexcept>
#include <optional>

class EnvValidationError : public std::runtime_error {
public:
    explicit EnvValidationError(const std::string& message)
        : std::runtime_error(message) {}
};

class EnvValidator {
public:
    static void validate();
    
private:
    static void validateHttpConfig();
    static void validateGrpcConfig();
    static void validateKrakenDConfig();
    
    static std::string getRequiredEnv(const std::string& name);
    static std::optional<std::string> getOptionalEnv(const std::string& name);
    static int getIntEnv(const std::string& name, int defaultValue);
    static bool getBoolEnv(const std::string& name, bool defaultValue);
    
    static bool isValidPort(int port);
    static bool isValidHost(const std::string& host);
    static bool isValidTimeout(int timeout);
    static bool isValidRetries(int retries);
    static bool isValidRetryDelay(int delay);
    static bool isValidLogLevel(const std::string& level);
}; 