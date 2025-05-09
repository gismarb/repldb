#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <string>

class DBManager {
public:
    DBManager();
    void inicializarBanco();
    void adicionarReplica(const std::string& fonte, const std::string& destino, const std::string& agendamento);
    void removerReplica(const std::string& id);
    void listarReplicas();
    void listarLogs(const std::string& id = "");
    void executarReplica(const std::string& id);
    void executarTodas();

private:
    std::string gerarComandoExecucao(const std::string& id);
};

#endif // DBMANAGER_H

