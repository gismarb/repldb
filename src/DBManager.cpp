#include "DBManager.h"
#include "DBHelper.h"
#include "Replicator.h"
#include "CronHelper.h"
#include <iostream>
#include <sstream>
#include <fstream> // ERREI AQUI
#include <cstdlib>
#include "Utils.h" // Utils::trim
#include <algorithm> // necessario std::all_of 
#include <unistd.h>  // Necessário para readlink()

// PENDENTE REFATORACAO (LINGUA)

DBManager::DBManager() {}

void DBManager::inicializarBanco() {
    if (!DBHelper::criarBancoDeDados()) {
        std::cerr << "[repldb] Falha ao criar banco de dados.\n";
        return;
    }

    std::cout << "[repldb] Executando estrutura do banco...\n";
    if (DBHelper::executeSQL("db/init.sql")) {
        std::cout << "[repldb] Banco inicializado com sucesso!\n";
    } else {
        std::cerr << "[repldb] Erro ao executar init.sql\n";
    }
} // ERREI AQUI, INCLUINDO ESTRUTURA PARA CRIACAO DO BANCO INICIALMENTE

void DBManager::adicionarReplica(const std::string& fonte, const std::string& destino, const std::string& agendamento) {
    std::cout << "[repldb] Adicionando plano de replicação...\n";

    if (!DBHelper::insertReplicationPlan(fonte, destino, agendamento)) {
        std::cerr << "[repldb] Erro ao inserir plano de replicação.\n";
        return;
    }

    // Obter o último ID inserido corretamente
    std::string getIdCmd = "echo \"SELECT MAX(id) FROM planos_replicacao;\" | "
                           "/opt/firebird/bin/isql -user SYSDBA -password repl@gis123 "
                           "/opt/firebird/data/repldb.fdb -q > /tmp/repldb_last_id.txt";
    std::system(getIdCmd.c_str());

    std::ifstream file("/tmp/repldb_last_id.txt");
    std::string line, id;
    while (std::getline(file, line)) {
        line = Utils::trim(line);
        if (!line.empty() && std::all_of(line.begin(), line.end(), ::isdigit)) {
            id = line;
            break;
        }
    }
    file.close();
    std::remove("/tmp/repldb_last_id.txt");

    if (id.empty()) {
        std::cerr << "[repldb] Falha ao identificar o ID do plano criado.\n";
        return;
    }

    std::cout << "[repldb] Plano registrado com ID: " << id << "\n";

    // Se houver agendamento, cria no cron com caminho dinâmico
    if (!agendamento.empty()) {
        char realPath[1024];
        ssize_t count = readlink("/proc/self/exe", realPath, sizeof(realPath));
        std::string execPath = (count != -1) ? std::string(realPath, count) : "./repldb";

        std::ostringstream comando;
        comando << execPath << " --run-replica --id " << id;

        if (!CronHelper::addCronJob(agendamento, comando.str(), id)) {
            std::cerr << "[repldb] Falha ao agendar tarefa via cron.\n";
        } else {
            std::cout << "[repldb] Agendamento criado com sucesso.\n";
        }
    }
}

void DBManager::removerReplica(const std::string& id) {
    std::cout << "[repldb] Removendo plano ID " << id << "...\n";

    // Caminho dinâmico do binário atual
    char realPath[1024];
    ssize_t count = readlink("/proc/self/exe", realPath, sizeof(realPath));
    std::string execPath = (count != -1) ? std::string(realPath, count) : "./repldb";

    // Remover agendamento do cron
    if (!CronHelper::removeCronJob(execPath, id)) {
        std::cerr << "[repldb] Aviso: nenhuma tarefa agendada foi encontrada para o ID " << id << ".\n";
    } else {
        std::cout << "[repldb] Agendamento removido com sucesso.\n";
    }

    // Comando para remover plano do banco
    std::ostringstream sql;
    sql << "DELETE FROM planos_replicacao WHERE id = " << id << ";";
    if (DBHelper::executeSQLCommand(sql.str())) {
        std::cout << "[repldb] Plano excluído com sucesso.\n";
    } else {
        std::cerr << "[repldb] Erro ao remover plano do banco.\n";
    }
}


void DBManager::listarReplicas() {
    std::cout << "[repldb] Listando planos de replicação...\n";
    DBHelper::listReplicationPlans();
}

void DBManager::listarLogs(const std::string& id) {
    std::cout << "[repldb] Listando logs...\n";
    DBHelper::listReplicationLogs(id);
}

void DBManager::executarReplica(const std::string& id) {
    std::ostringstream sql;
    sql << "SELECT origem, destino FROM planos_replicacao WHERE id = " << id << ";";

    std::string tmpFile = "/tmp/repldb_exec_" + id + ".sql";
    std::ofstream out(tmpFile);
    out << sql.str();
    out.close();

    std::string extractCmd = "/opt/firebird/bin/isql -user SYSDBA -password repl@gis123 "
                             "/opt/firebird/data/repldb.fdb -i " + tmpFile + " > /tmp/repldb_paths.txt";
    std::system(extractCmd.c_str());
    std::remove(tmpFile.c_str());

    std::ifstream result("/tmp/repldb_paths.txt");
    std::string linha, origem, destino;
    while (std::getline(result, linha)) {
        linha = Utils::trim(linha);
        if (!linha.empty() && linha.find('/') != std::string::npos) {
            std::istringstream iss(linha);
            std::string token;
            while (iss >> token) {
                if (token[0] == '/') {
                    if (origem.empty()) origem = token;
                    else if (destino.empty()) {
                        destino = token;
                        break;
                    }
                }
            }
        }
    }

    result.close();
    std::remove("/tmp/repldb_paths.txt");

    if (origem.empty() || destino.empty()) {
        std::cerr << "[repldb] Erro: não foi possível obter origem e destino do plano " << id << ".\n";
        DBHelper::insertReplicationLog(id, "ERRO", "Origem ou destino ausente ou inválido.");
        return;
    }

    std::cout << "[repldb] Executando backup do plano " << id << "...\n";
    if (Replicator::executarBackup(origem, "/tmp/repldb_backup.fbk")) {
        std::cout << "[repldb] Executando restore para " << destino << "...\n";
        if (Replicator::executarRestore("/tmp/repldb_backup.fbk", destino)) {
            DBHelper::insertReplicationLog(id, "SUCESSO", "Replicação realizada com sucesso.");
        } else {
            DBHelper::insertReplicationLog(id, "ERRO", "Falha ao restaurar a réplica.");
        }
    } else {
        DBHelper::insertReplicationLog(id, "ERRO", "Falha ao gerar backup.");
    }

    std::remove("/tmp/repldb_backup.fbk");
}



void DBManager::executarTodas() {
    std::string cmd = "echo \"SELECT id FROM planos_replicacao;\" | "
                      "/opt/firebird/bin/isql -user SYSDBA -password repl@gis123 "
                      "/opt/firebird/data/repldb.fdb -q > /tmp/repldb_all_ids.txt";
    std::system(cmd.c_str());

    std::ifstream file("/tmp/repldb_all_ids.txt");
    std::string linha;
    while (std::getline(file, linha)) {
        linha = Utils::trim(linha);
        if (!linha.empty() && std::all_of(linha.begin(), linha.end(), ::isdigit)) {
            executarReplica(linha);
        }
    }
    file.close();
    std::remove("/tmp/repldb_all_ids.txt");
}
