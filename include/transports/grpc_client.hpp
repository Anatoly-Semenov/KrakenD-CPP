#pragma once

#include <string>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "krakend.grpc.pb.h"

class GrpcClient {
public:
    GrpcClient(const std::string& server_address);
    
    // Пересылка HTTP запроса через gRPC
    crow::response forward_request(const crow::request& req);

private:
    std::unique_ptr<krakend::KrakenDService::Stub> stub_;
}; 