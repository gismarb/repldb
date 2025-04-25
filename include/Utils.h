#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    std::string obterTimestampAtual();
    bool arquivoExiste(const std::string& caminho);
    std::string trim(const std::string& str);
    std::vector<std::string> dividir(const std::string& str, char delimitador);
}

#endif // UTILS_H
