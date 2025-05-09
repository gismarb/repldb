#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static bool isNumeric(const std::string& str);
    static std::string trim(const std::string& str);
};

#endif // UTILS_H

