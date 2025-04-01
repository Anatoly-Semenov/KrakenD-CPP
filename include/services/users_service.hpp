#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "http_client.hpp"

class UsersService {
public:
    UsersService(const std::string& base_url);
    
    nlohmann::json login(const std::string& username, const std::string& password);
    void logout(const std::string& token);
    nlohmann::json register_user(const std::string& username, 
                                const std::string& password,
                                const std::string& email);
    nlohmann::json refresh_token(const std::string& refresh_token);

private:
    std::string base_url_;
    HttpClient http_client_;
}; 