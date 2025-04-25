#include "DBManager.h"
#include <iostream>
#include <string>
#include <fstream>

void exibirAjuda() {
    std::cout << "\nUso do repldb:\n"
              << "  --add-replica --fonte <caminho> --destino <caminho>\n"
              << "  --remove-replica --id <id>\n"
              << "  --list-replica\n"
              << "  --run-replica --id <id> | --all\n"
              << "  --list-logs [--id <id>]\n"
              << "  --init-db\n"
              << "  --help\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        exibirAjuda();
        return 1;
    }

    DBManager dbm;
    std::string comando = argv[1];

    try {
        if (comando == "--init-db") {
            dbm.criarEstruturaBanco();
        }
        else if (comando == "--add-replica") {
            std::string fonte, destino;
            for (int i = 2; i < argc; i++) {
                if (std::string(argv[i]) == "--fonte" && i + 1 < argc) fonte = argv[++i];
                else if (std::string(argv[i]) == "--destino" && i + 1 < argc) destino = argv[++i];
            }
            dbm.adicionarPlano(fonte, destino);
        }
        else if (comando == "--remove-replica") {
            int id = -1;
            for (int i = 2; i < argc; i++) {
                if (std::string(argv[i]) == "--id" && i + 1 < argc) id = std::stoi(argv[++i]);
            }
            dbm.removerPlano(id);
        }
        else if (comando == "--list-replica") {
            dbm.listarPlanos();
        }
        else if (comando == "--run-replica") {
            if (argc >= 3 && std::string(argv[2]) == "--all") {
                dbm.executarTodosPlanos();
            } else {
                int id = -1;
                for (int i = 2; i < argc; i++) {
                    if (std::string(argv[i]) == "--id" && i + 1 < argc) id = std::stoi(argv[++i]);
                }
                dbm.executarPlano(id);
            }
        }
        else if (comando == "--list-logs") {
            int id = -1;
            for (int i = 2; i < argc; i++) {
                if (std::string(argv[i]) == "--id" && i + 1 < argc) id = std::stoi(argv[++i]);
            }
            dbm.listarLogs(id);
        }
        else if (comando == "--help") {
            exibirAjuda();
        }
        else {
            std::cerr << "Comando desconhecido.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
