#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <string>

class DBManager {
public:
    DBManager();
    ~DBManager();

    void criarEstruturaBanco();

    void adicionarPlano(const std::string& fonte, const std::string& destino);
    void removerPlano(int id);
    void listarPlanos();
    void executarPlano(int id);
    void executarTodosPlanos();
    void listarLogs(int id = -1);

private:
    void logOperacao(int plano_id, const std::string& mensagem);
};

#endif // DBMANAGER_H
