#ifndef DBHELPER_H
#define DBHELPER_H

#include <string>

// PENDENTE REFATORACAO (LINGUA)

class DBHelper {
public:
    static bool criarBancoDeDados(); // <-- Adicionado
    static bool executeSQL(const std::string& sqlFile);
    static bool executeSQLCommand(const std::string& sqlCommand);
    static bool insertReplicationPlan(const std::string& fonte, const std::string& destino, const std::string& agendamento, const std::string& tipo = "backup_restore");
    static bool insertReplicationLog(const std::string& planoId, const std::string& status, const std::string& mensagem);
    static void listReplicationPlans();
    static void listReplicationLogs(const std::string& planoId = "");
    static bool registrarLog(const std::string& planoId, const std::string& status, const std::string& mensagem);

};

#endif // DBHELPER_H
