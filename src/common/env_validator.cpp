#include "common/env_validator.hpp"
#include <cstdlib>
#include <regex>

void EnvValidator::validate() {
    validateHttpConfig();
    validateGrpcConfig();
    validateKrakenDConfig();
}

void EnvValidator::validateHttpConfig() {
    std::string host = getRequiredEnv("HTTP_HOST");
    if (!isValidHost(host)) {
        throw EnvValidationError("Invalid HTTP_HOST format");
    }

    int port = getIntEnv("HTTP_PORT", 8080);
    if (!isValidPort(port)) {
        throw EnvValidationError("Invalid HTTP_PORT value");
    }
}

void EnvValidator::validateGrpcConfig() {
    std::string address = getRequiredEnv("GRPC_ADDRESS");
    if (address.empty()) {
        throw EnvValidationError("GRPC_ADDRESS is required");
    }
}

void EnvValidator::validateKrakenDConfig() {
    std::string host = getRequiredEnv("KRAKEND_HOST");
    if (!isValidHost(host)) {
        throw EnvValidationError("Invalid KRAKEND_HOST format");
    }

    int port = getIntEnv("KRAKEND_PORT", 8081);
    if (!isValidPort(port)) {
        throw EnvValidationError("Invalid KRAKEND_PORT value");
    }

    int timeout = getIntEnv("KRAKEND_TIMEOUT", 30);
    if (!isValidTimeout(timeout)) {
        throw EnvValidationError("Invalid KRAKEND_TIMEOUT value");
    }

    int maxRetries = getIntEnv("KRAKEND_MAX_RETRIES", 3);
    if (!isValidRetries(maxRetries)) {
        throw EnvValidationError("Invalid KRAKEND_MAX_RETRIES value");
    }

    int retryDelay = getIntEnv("KRAKEND_RETRY_DELAY", 1);
    if (!isValidRetryDelay(retryDelay)) {
        throw EnvValidationError("Invalid KRAKEND_RETRY_DELAY value");
    }

    bool sslVerify = getBoolEnv("KRAKEND_SSL_VERIFY", true);
    if (sslVerify) {
        std::string certPath = getRequiredEnv("KRAKEND_SSL_CERT_PATH");
        std::string keyPath = getRequiredEnv("KRAKEND_SSL_KEY_PATH");
        if (certPath.empty() || keyPath.empty()) {
            throw EnvValidationError("SSL certificate and key paths are required when SSL verification is enabled");
        }
    }

    std::string logLevel = getRequiredEnv("KRAKEND_LOG_LEVEL");
    if (!isValidLogLevel(logLevel)) {
        throw EnvValidationError("Invalid KRAKEND_LOG_LEVEL value");
    }
}

std::string EnvValidator::getRequiredEnv(const std::string& name) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        throw EnvValidationError("Required environment variable " + name + " is not set");
    }
    return std::string(value);
}

std::optional<std::string> EnvValidator::getOptionalEnv(const std::string& name) {
    const char* value = std::getenv(name.c_str());
    return value ? std::optional<std::string>(value) : std::nullopt;
}

int EnvValidator::getIntEnv(const std::string& name, int defaultValue) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        return defaultValue;
    }
    try {
        return std::stoi(value);
    } catch (const std::exception&) {
        throw EnvValidationError("Invalid integer value for " + name);
    }
}

bool EnvValidator::getBoolEnv(const std::string& name, bool defaultValue) {
    const char* value = std::getenv(name.c_str());
    if (!value) {
        return defaultValue;
    }
    std::string strValue(value);
    std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
    return strValue == "true" || strValue == "1";
}

bool EnvValidator::isValidPort(int port) {
    return port > 0 && port <= 65535;
}

bool EnvValidator::isValidHost(const std::string& host) {
    if (host == "0.0.0.0" || host == "localhost" || host == "127.0.0.1") {
        return true;
    }
    std::regex hostRegex("^([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,}$");
    return std::regex_match(host, hostRegex);
}

bool EnvValidator::isValidTimeout(int timeout) {
    return timeout > 0 && timeout <= 300; // Максимум 5 минут
}

bool EnvValidator::isValidRetries(int retries) {
    return retries >= 0 && retries <= 10;
}

bool EnvValidator::isValidRetryDelay(int delay) {
    return delay >= 0 && delay <= 60; // Максимум 1 минута
}

bool EnvValidator::isValidLogLevel(const std::string& level) {
    static const std::vector<std::string> validLevels = {
        "debug", "info", "warning", "error", "critical"
    };
    std::string lowerLevel = level;
    std::transform(lowerLevel.begin(), lowerLevel.end(), lowerLevel.begin(), ::tolower);
    return std::find(validLevels.begin(), validLevels.end(), lowerLevel) != validLevels.end();
} 