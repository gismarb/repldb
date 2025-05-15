#ifndef REPLICATOR_H
#define REPLICATOR_H

#include <string>

// PENDENTE REFATORACAO (LINGUA)

class Replicator {
public:
    static bool executarBackup(const std::string& origem, const std::string& destinoTmp);
    static bool executarRestore(const std::string& destinoTmp, const std::string& destinoFinal);
    static bool copiarParaRemoto(const std::string& origem, const std::string& destinoHost, const std::string& destinoPath);
    static bool executarRemoto(const std::string& host, const std::string& comando);
};

#endif // REPLICATOR_H

