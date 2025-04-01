#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "http_client.hpp"

class PaymentsService {
public:
    PaymentsService(const std::string& base_url);
    
    nlohmann::json create_payment(const std::string& amount,
                                const std::string& currency,
                                const std::string& description);
    
    void handle_callback(const std::string& payment_id,
                        const std::string& status,
                        const std::string& signature);
    
    nlohmann::json check_payment_status(const std::string& payment_id);

private:
    std::string base_url_;
    HttpClient http_client_;
}; 