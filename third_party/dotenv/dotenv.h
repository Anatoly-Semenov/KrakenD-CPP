#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <map>

namespace dotenv {

class env {
public:
    static void init(const std::string& path = ".env") {
        std::ifstream file(path);
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                // Remove quotes if present
                if (value.front() == '"' && value.back() == '"') {
                    value = value.substr(1, value.length() - 2);
                }

                // Trim whitespace
                key = trim(key);
                value = trim(value);

                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }

private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
};

} // namespace dotenv 