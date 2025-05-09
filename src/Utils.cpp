#include "Utils.h"
#include <cctype>
#include <algorithm>

bool Utils::isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return str.substr(first, (last - first + 1));
}

