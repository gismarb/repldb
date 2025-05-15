#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <unordered_map>

// PENDENTE REFATORACAO (LINGUA)

class ArgumentParser {
public:
    ArgumentParser();
    void parse(int argc, char* argv[]);
    bool hasFlag(const std::string& flag) const;
    std::string getValue(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> argumentos;
};

#endif // ARGUMENT_PARSER_H

