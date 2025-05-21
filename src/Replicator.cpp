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

bool Replicator::executarRestore(const std::string& origem, const std::string& destino) {
    std::cout << "[repldb] Executando replicação de: " << origem << " → " << destino << "\n";

    std::ostringstream cmd;
    size_t pos = destino.find(':');
    bool isRemoto = (pos != std::string::npos && destino.find('/') > pos);

    if (isRemoto) {
        // Destino remoto detectado
        std::string hostRemoto = destino.substr(0, pos);
        std::string caminhoRemoto = destino.substr(pos + 1);

        std::cout << "[repldb] Modo remoto ativado via Service Manager: " << hostRemoto << "\n";

        cmd << "/opt/firebird/bin/gbak -b -se localhost:service_mgr -g "
            << "-user SYSDBA -password repl@gis123 "
            << origem << " stdout | "
            << "/opt/firebird/bin/gbak -rep -se " << hostRemoto << ":service_mgr "
            << "-user SYSDBA -password repl@gis123 "
            << "stdin " << caminhoRemoto;
    } else {
        // Replicação local padrão
        cmd << "/opt/firebird/bin/gbak -c -replace_database "
            << "-user SYSDBA -password repl@gis123 "
            << origem << " " << destino;
    }

    std::cout << "[repldb] Comando: " << cmd.str() << "\n";
    int result = std::system(cmd.str().c_str());

    // Se for local e o restore foi bem-sucedido, ajusta a permissão
    if (!isRemoto && result == 0) {
        std::ostringstream chownCmd;
        chownCmd << "chown firebird:firebird " << destino;
        int chownRet = std::system(chownCmd.str().c_str());
        if (chownRet != 0) {
            std::cerr << "[repldb] Aviso: falha ao ajustar permissão para firebird:firebird.\n";
        } else {
            std::cout << "[repldb] Permissão ajustada com sucesso para firebird:firebird.\n";
        }
    }

    return (result == 0);
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

