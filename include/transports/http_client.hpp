#pragma once

#include <string>
#include <map>
#include <memory>
#include <curl/curl.h>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string post(const std::string& url, 
                    const std::string& data,
                    const std::map<std::string, std::string>& headers = {});
    
    std::string get(const std::string& url,
                   const std::map<std::string, std::string>& headers = {});

private:
    CURL* curl_;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    void setHeaders(const std::map<std::string, std::string>& headers);
}; 