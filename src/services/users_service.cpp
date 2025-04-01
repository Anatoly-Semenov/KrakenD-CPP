#include "users_service.hpp"
#include "validator.hpp"
#include <stdexcept>

UsersService::UsersService(const std::string& base_url) : base_url_(base_url) {}

nlohmann::json UsersService::login(const std::string& username, const std::string& password) {

    if (username.empty()) {
        throw ValidationError("Имя пользователя не может быть пустым");
    }
    Validator::validatePassword(password);
    
    nlohmann::json request_data = {
        {"username", username},
        {"password", password}
    };
    
    std::string response = http_client_.post(
        base_url_ + "/login",
        request_data.dump(),
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
    auto json_response = nlohmann::json::parse(response);

    if (!json_response.contains("access_token") || !json_response.contains("refresh_token")) {
        throw ValidationError("Некорректный формат ответа от сервера");
    }

    Validator::validateToken(json_response["access_token"]);
    Validator::validateToken(json_response["refresh_token"]);
    
    return json_response;
}

void UsersService::logout(const std::string& token) {

    Validator::validateToken(token);
    
    std::string response = http_client_.post(
        base_url_ + "/logout",
        "",
        {
            {"Authorization", "Bearer " + token},
            {"Content-Type", "application/json"}
        }
    );

    Validator::validateResponse(response);
}

nlohmann::json UsersService::register_user(const std::string& username,
                                         const std::string& password,
                                         const std::string& email) {

    if (username.empty()) {
        throw ValidationError("Имя пользователя не может быть пустым");
    }
    Validator::validatePassword(password);
    Validator::validateEmail(email);
    
    nlohmann::json request_data = {
        {"username", username},
        {"password", password},
        {"email", email}
    };
    
    std::string response = http_client_.post(
        base_url_ + "/register",
        request_data.dump(),
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
    auto json_response = nlohmann::json::parse(response);

    if (!json_response.contains("user_id") || !json_response.contains("username")) {
        throw ValidationError("Некорректный формат ответа от сервера");
    }
    
    return json_response;
}

nlohmann::json UsersService::refresh_token(const std::string& refresh_token) {

    Validator::validateToken(refresh_token);
    
    nlohmann::json request_data = {
        {"refresh_token", refresh_token}
    };
    
    std::string response = http_client_.post(
        base_url_ + "/refresh",
        request_data.dump(),
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
    auto json_response = nlohmann::json::parse(response);

    if (!json_response.contains("access_token")) {
        throw ValidationError("Некорректный формат ответа от сервера");
    }

    Validator::validateToken(json_response["access_token"]);
    
    return json_response;
} 