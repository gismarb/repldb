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
    // Gera arquivo de input SQL para isql
    std::ofstream sqlfile("/tmp/repldb_replica_input.sql");
    sqlfile << "SET LIST ON;\n";
    sqlfile << "SELECT origem, destino FROM planos_replicacao WHERE id = " << id << ";\n";
    sqlfile << "QUIT;\n";
    sqlfile.close();

    // Executa isql com saída redirecionada para arquivo
    std::ostringstream cmd;
    cmd << "/opt/firebird/bin/isql -user SYSDBA -password repl@gis123 /opt/firebird/data/repldb.fdb "
        << "-q -nod -i /tmp/repldb_replica_input.sql -o /tmp/repldb_replica_out.txt";
    std::system(cmd.str().c_str());

    std::remove("/tmp/repldb_replica_input.sql");

    // Lê os dados de origem e destino da saída
    std::ifstream file("/tmp/repldb_replica_out.txt");
    std::string line, origem, destino;

    while (std::getline(file, line)) {
        line = Utils::trim(line);
        if (line.find("ORIGEM") == 0) {
            origem = Utils::trim(line.substr(6));
        } else if (line.find("DESTINO") == 0) {
            destino = Utils::trim(line.substr(7));
        }
    }

    file.close();
    std::remove("/tmp/repldb_replica_out.txt");

    if (origem.empty() || destino.empty()) {
        std::cerr << "[repldb] Erro: não foi possível obter origem e destino do plano " << id << ".\n";
        DBHelper::registrarLog(id, "ERRO", "Origem ou destino ausente ou inválido.");
        return;
    }

    std::cout << "[repldb] Executando backup do plano " << id << "...\n";
    std::string backupTmp = "/tmp/repldb_backup.fbk";

    if (!Replicator::executarBackup(origem, backupTmp)) {
        std::cerr << "[repldb] Falha ao gerar backup.\n";
        DBHelper::registrarLog(id, "ERRO", "Falha ao gerar backup.");
        return;
    }

    std::cout << "[repldb] Executando restore...\n";

    // Detecta se é remoto
    bool isRemoto = destino.find(':') != std::string::npos && destino.find('/') > destino.find(':');
    bool sucessoRestore = false;

    if (isRemoto) {
        // usa a origem original
        sucessoRestore = Replicator::executarRestore(origem, destino);
    } else {
        // usa o backup gerado
        sucessoRestore = Replicator::executarRestore(backupTmp, destino);
    }

    if (!sucessoRestore) {
        std::cerr << "[repldb] Falha ao restaurar.\n";
        DBHelper::registrarLog(id, "ERRO", "Falha ao restaurar.");
        return;
    }

    DBHelper::registrarLog(id, "SUCESSO", "Replicação concluída com sucesso.");
    std::cout << "[repldb] Replicação do plano " << id << " concluída com sucesso.\n";
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
