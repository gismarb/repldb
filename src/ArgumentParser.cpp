#include "ArgumentParser.h"

ArgumentParser::ArgumentParser() {}

void ArgumentParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string flag = argv[i];
        if (flag.rfind("--", 0) == 0) {
            std::string value = "";
            if ((i + 1) < argc && std::string(argv[i + 1]).rfind("--", 0) != 0) {
                value = argv[i + 1];
                ++i;
            }
            argumentos[flag] = value;
        }
    }
}

bool ArgumentParser::hasFlag(const std::string& flag) const {
    return argumentos.find(flag) != argumentos.end();
}

std::string ArgumentParser::getValue(const std::string& key) const {
    auto it = argumentos.find(key);
    if (it != argumentos.end()) {
        return it->second;
    }
    return "";
}

