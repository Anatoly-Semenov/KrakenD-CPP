#include "payments_service.hpp"
#include "validator.hpp"
#include <stdexcept>

PaymentsService::PaymentsService(const std::string& base_url) : base_url_(base_url) {}

nlohmann::json PaymentsService::create_payment(const std::string& amount,
                                             const std::string& currency,
                                             const std::string& description) {

    Validator::validateAmount(amount);
    Validator::validateCurrency(currency);
    if (description.empty()) {
        throw ValidationError("Описание платежа не может быть пустым");
    }
    
    nlohmann::json request_data = {
        {"amount", amount},
        {"currency", currency},
        {"description", description}
    };
    
    std::string response = http_client_.post(
        base_url_ + "/payments",
        request_data.dump(),
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
    auto json_response = nlohmann::json::parse(response);

    if (!json_response.contains("payment_id") || !json_response.contains("status")) {
        throw ValidationError("Некорректный формат ответа от сервера");
    }
    
    return json_response;
}

void PaymentsService::handle_callback(const std::string& payment_id,
                                    const std::string& status,
                                    const std::string& signature) {

    if (payment_id.empty()) {
        throw ValidationError("ID платежа не может быть пустым");
    }
    if (status.empty()) {
        throw ValidationError("Статус платежа не может быть пустым");
    }
    if (signature.empty()) {
        throw ValidationError("Подпись не может быть пустой");
    }

    static const std::set<std::string> valid_statuses = {"success", "failed", "pending"};
    if (valid_statuses.find(status) == valid_statuses.end()) {
        throw ValidationError("Недопустимый статус платежа");
    }
    
    nlohmann::json request_data = {
        {"payment_id", payment_id},
        {"status", status},
        {"signature", signature}
    };
    
    std::string response = http_client_.post(
        base_url_ + "/payments/callback",
        request_data.dump(),
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
}

nlohmann::json PaymentsService::check_payment_status(const std::string& payment_id) {

    if (payment_id.empty()) {
        throw ValidationError("ID платежа не может быть пустым");
    }
    
    std::string response = http_client_.get(
        base_url_ + "/payments/" + payment_id + "/status",
        {{"Content-Type", "application/json"}}
    );

    Validator::validateResponse(response);
    auto json_response = nlohmann::json::parse(response);

    if (!json_response.contains("status") || !json_response.contains("payment_id")) {
        throw ValidationError("Некорректный формат ответа от сервера");
    }

    static const std::set<std::string> valid_statuses = {"success", "failed", "pending"};
    if (valid_statuses.find(json_response["status"]) == valid_statuses.end()) {
        throw ValidationError("Недопустимый статус платежа в ответе");
    }
    
    return json_response;
} 