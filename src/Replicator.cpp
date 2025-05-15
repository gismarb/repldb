#include "Replicator.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <filesystem>

// PENDENTE REFATORACAO (LINGUA)

namespace fs = std::filesystem;

bool Replicator::executarBackup(const std::string& origem, const std::string& destinoTmp) {
    std::ostringstream cmd;
    cmd << "/opt/firebird/bin/gbak -b -g -user SYSDBA -password repl@gis123 "
        << origem << " " << destinoTmp;

    std::cout << "[repldb] Executando backup: " << cmd.str() << std::endl;
    return std::system(cmd.str().c_str()) == 0;
}

bool Replicator::executarRestore(const std::string& destinoTmp, const std::string& destinoFinal) {
    std::ostringstream cmd;
    cmd << "/opt/firebird/bin/gbak -c -replace_database -user SYSDBA -password repl@gis123 "
        << destinoTmp << " " << destinoFinal;

    std::cout << "[repldb] Executando restore: " << cmd.str() << std::endl;
    return std::system(cmd.str().c_str()) == 0;
}

bool Replicator::copiarParaRemoto(const std::string& origem, const std::string& destinoHost, const std::string& destinoPath) {
    std::ostringstream cmd;
    cmd << "scp " << origem << " " << destinoHost << ":" << destinoPath;

    std::cout << "[repldb] Copiando backup para host remoto: " << cmd.str() << std::endl;
    return std::system(cmd.str().c_str()) == 0;
}

bool Replicator::executarRemoto(const std::string& host, const std::string& comando) {
    std::ostringstream cmd;
    cmd << "ssh " << host << " \"" << comando << "\"";

    std::cout << "[repldb] Executando comando remoto: " << cmd.str() << std::endl;
    return std::system(cmd.str().c_str()) == 0;
}

