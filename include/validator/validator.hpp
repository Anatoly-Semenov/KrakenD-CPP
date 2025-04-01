#pragma once

#include <string>
#include <regex>
#include <stdexcept>

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message) : std::runtime_error(message) {}
};

class Validator {
public:
    static void validateEmail(const std::string& email) {
        static const std::regex email_regex(
            R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})"
        );
        if (!std::regex_match(email, email_regex)) {
            throw ValidationError("Некорректный email формат");
        }
    }

    static void validatePassword(const std::string& password) {
        if (password.length() < 8) {
            throw ValidationError("Пароль должен содержать минимум 8 символов");
        }
        if (!std::regex_search(password, std::regex("[A-Z]"))) {
            throw ValidationError("Пароль должен содержать хотя бы одну заглавную букву");
        }
        if (!std::regex_search(password, std::regex("[a-z]"))) {
            throw ValidationError("Пароль должен содержать хотя бы одну строчную букву");
        }
        if (!std::regex_search(password, std::regex("[0-9]"))) {
            throw ValidationError("Пароль должен содержать хотя бы одну цифру");
        }
    }

    static void validateAmount(const std::string& amount) {
        static const std::regex amount_regex(R"(\d+(\.\d{1,2})?)");
        if (!std::regex_match(amount, amount_regex)) {
            throw ValidationError("Некорректный формат суммы платежа");
        }
        double value = std::stod(amount);
        if (value <= 0) {
            throw ValidationError("Сумма платежа должна быть больше 0");
        }
    }

    static void validateCurrency(const std::string& currency) {
        static const std::set<std::string> valid_currencies = {"USD", "EUR", "RUB"};
        if (valid_currencies.find(currency) == valid_currencies.end()) {
            throw ValidationError("Неподдерживаемая валюта");
        }
    }

    static void validateToken(const std::string& token) {
        if (token.empty()) {
            throw ValidationError("Токен не может быть пустым");
        }
        if (token.length() < 32) {
            throw ValidationError("Некорректный формат токена");
        }
    }

    static void validateResponse(const std::string& response) {
        if (response.empty()) {
            throw ValidationError("Пустой ответ от сервера");
        }
    }
}; 