#include "DBHelper.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#define DB_PATH "/opt/firebird/data/repldb.fdb"
#define ISQL_CMD "/opt/firebird/bin/isql"

bool DBHelper::criarBancoDeDados() {
    if (std::filesystem::exists(DB_PATH)) {
        std::cout << "[repldb] Banco já existe: " << DB_PATH << "\n";
        return true;
    }

    std::string cmd = "echo \"CREATE DATABASE '" + std::string(DB_PATH) + "';\" | "
                      ISQL_CMD " -user SYSDBA -password repl@gis123 -q";
    std::cout << "[repldb] Criando banco com comando:\n" << cmd << "\n";

    return std::system(cmd.c_str()) == 0;
}

bool DBHelper::executeSQL(const std::string& sqlFile) {
    std::ostringstream cmd;
    cmd << ISQL_CMD << " -user SYSDBA -password repl@gis123 " << DB_PATH << " -i " << sqlFile;
    return std::system(cmd.str().c_str()) == 0;
}

bool DBHelper::executeSQLCommand(const std::string& sqlCommand) {
    std::string tmpFile = "/tmp/repldb_temp.sql";
    std::ofstream out(tmpFile);
    if (!out) {
        std::cerr << "[repldb] Erro ao criar arquivo temporário para SQL.\n";
        return false;
    }

    out << sqlCommand << std::endl;
    out.close();

    bool result = executeSQL(tmpFile);
    std::remove(tmpFile.c_str());
    return result;
}

bool DBHelper::insertReplicationPlan(const std::string& fonte, const std::string& destino, const std::string& agendamento, const std::string& tipo) {
    std::ostringstream sql;
    sql << "INSERT INTO planos_replicacao (origem, destino, agendamento, tipo_replicacao) VALUES ("
        << "'" << fonte << "', "
        << "'" << destino << "', "
        << (agendamento.empty() ? "NULL" : "'" + agendamento + "'") << ", "
        << "'" << tipo << "');";
    return executeSQLCommand(sql.str());
}

bool DBHelper::insertReplicationLog(const std::string& planoId, const std::string& status, const std::string& mensagem) {
    std::ostringstream sql;
    sql << "INSERT INTO log_replicacao (plano_id, status, mensagem) VALUES ("
        << planoId << ", '" << status << "', '" << mensagem << "');";
    return executeSQLCommand(sql.str());
}

void DBHelper::listReplicationPlans() {
    std::string cmd = "echo \"SELECT id, origem, destino, agendamento, tipo_replicacao FROM planos_replicacao ORDER BY id;\" | "
                      ISQL_CMD " -user SYSDBA -password masterkey " DB_PATH " -q -e";
    std::system(cmd.c_str());
}


void DBHelper::listReplicationLogs(const std::string& planoId) {
    std::ostringstream sql;
    sql << "SELECT id, plano_id, inicio, fim, status, mensagem FROM log_replicacao";
    if (!planoId.empty()) {
        sql << " WHERE plano_id = " << planoId;
    }
    sql << " ORDER BY inicio DESC;";

    std::ostringstream cmd;
    cmd << "echo \"" << sql.str() << "\" | "
        << ISQL_CMD << " -user SYSDBA -password masterkey " << DB_PATH << " -q -e";
    std::system(cmd.str().c_str());
}
