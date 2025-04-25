#include "Utils.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace Utils {

    std::string obterTimestampAtual() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    bool arquivoExiste(const std::string& caminho) {
        std::ifstream arquivo(caminho);
        return arquivo.good();
    }

    std::string trim(const std::string& str) {
        auto inicio = str.begin();
        while (inicio != str.end() && std::isspace(*inicio)) ++inicio;

        auto fim = str.end();
        do {
            --fim;
        } while (std::distance(inicio, fim) > 0 && std::isspace(*fim));

        return std::string(inicio, fim + 1);
    }

    std::vector<std::string> dividir(const std::string& str, char delimitador) {
        std::vector<std::string> partes;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimitador)) {
            partes.push_back(trim(item));
        }
        return partes;
    }

}
