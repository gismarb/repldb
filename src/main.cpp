#include "ArgumentParser.h"
#include "DBManager.h"
#include <iostream>

// PENDENTE REFATORACAO (LINGUA)

void printUsage() {
    std::cout << "Uso:\n"
              << "     repldb --init-db\n"
              << "     repldb --add-replica --fonte <path> --destino <path> [--schedule \"<cron>\"]\n"
              << "     repldb --remove-replica --id <id>\n"
              << "     repldb --list-replica\n"
              << "     repldb --run-replica --id <id>\n"
              << "     repldb --run-replica --all\n"
              << "     repldb --list-logs [--id <id>]\n"
              << "     repldb --help\n";
}

int main(int argc, char* argv[]) {
    ArgumentParser parser;
    parser.parse(argc, argv);

    if (parser.hasFlag("--help") || argc == 1) {
        printUsage();
        return 0;
    }

    DBManager manager;

    try {
        if (parser.hasFlag("--init-db")) {
            manager.inicializarBanco();
        } else if (parser.hasFlag("--add-replica")) {
            std::string fonte = parser.getValue("--fonte");
            std::string destino = parser.getValue("--destino");
            std::string schedule = parser.getValue("--schedule");
            manager.adicionarReplica(fonte, destino, schedule);
        } else if (parser.hasFlag("--remove-replica")) {
            std::string id = parser.getValue("--id");
            manager.removerReplica(id);
        } else if (parser.hasFlag("--list-replica")) {
            manager.listarReplicas();
        } else if (parser.hasFlag("--run-replica")) {
            if (parser.hasFlag("--all")) {
                manager.executarTodas();
            } else {
                std::string id = parser.getValue("--id");
                manager.executarReplica(id);
            }
        } else if (parser.hasFlag("--list-logs")) {
            std::string id = parser.getValue("--id");
            manager.listarLogs(id);
        } else {
            printUsage();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

