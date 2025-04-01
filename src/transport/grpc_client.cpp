#include "grpc_client.hpp"
#include <iostream>

GrpcClient::GrpcClient(const std::string& server_address) {
    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    stub_ = krakend::KrakenDService::NewStub(channel);
}

crow::response GrpcClient::forward_request(const crow::request& req) {
    krakend::Request grpc_req;
    grpc_req.set_method(req.method);
    grpc_req.set_path(req.url);

    for (const auto& header : req.headers) {
        (*grpc_req.mutable_headers())[header.first] = header.second;
    }

    grpc_req.set_body(req.body);

    krakend::Response grpc_resp;
    grpc::ClientContext context;
    grpc::Status status = stub_->ForwardRequest(&context, grpc_req, &grpc_resp);

    if (!status.ok()) {
        std::cerr << "gRPC error: " << status.error_message() << std::endl;
        return crow::response(500);
    }

    crow::response resp;
    resp.code = grpc_resp.status_code();

    for (const auto& header : grpc_resp.headers()) {
        resp.set_header(header.first, header.second);
    }

    resp.body = grpc_resp.body();

    return resp;
} 