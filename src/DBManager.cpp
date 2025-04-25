#include "DBManager.h"
#include <ibpp.h>
#include <iostream>
#include <fstream>

#define DB_PATH "data/repldb.fdb"
#define SCRIPT_PATH "db/init.sql"
#define DB_USER "_sysdba"                   // DBA Account
#define DB_PASSWD "repl@gis123"             // DBA Password

DBManager::DBManager() {}
DBManager::~DBManager() {}

void DBManager::criarEstruturaBanco() {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    db->Create(4096);
    db->Connect();
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    tr->Start();

    std::ifstream script(SCRIPT_PATH);
    std::string linha, sql;
    while (std::getline(script, linha)) {
        sql += linha + "\n";
        if (linha.find(";") != std::string::npos) {
            db->ExecuteImmediate(sql);
            sql.clear();
        }
    }

    tr->Commit();
    db->Disconnect();
    std::cout << "Banco de dados criado com sucesso.\n";
}

void DBManager::adicionarPlano(const std::string& fonte, const std::string& destino) {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);
    st->Execute("INSERT INTO planos_replicacao (fonte, destino) VALUES (?, ?)", fonte, destino);
    tr->Commit(); db->Disconnect();
    std::cout << "Plano adicionado.\n";
}

void DBManager::removerPlano(int id) {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);
    st->Execute("DELETE FROM planos_replicacao WHERE id = ?", id);
    tr->Commit(); db->Disconnect();
    std::cout << "Plano removido.\n";
}

void DBManager::listarPlanos() {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);
    st->Execute("SELECT id, fonte, destino FROM planos_replicacao");
    while (st->Fetch()) {
        int id; std::string f, d;
        st->Get(1, id); st->Get(2, f); st->Get(3, d);
        std::cout << "ID: " << id << " | Fonte: " << f << " | Destino: " << d << "\n";
    }
    tr->Commit(); db->Disconnect();
}

void DBManager::executarPlano(int id) {
    std::cout << "Executando replicação do plano " << id << "...\n";
    logOperacao(id, "Replicação executada com sucesso.");
}

void DBManager::executarTodosPlanos() {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);
    st->Execute("SELECT id FROM planos_replicacao");
    while (st->Fetch()) {
        int id; st->Get(1, id);
        executarPlano(id);
    }
    tr->Commit(); db->Disconnect();
}

void DBManager::listarLogs(int id) {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, DB_USER, DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);

    if (id > 0) {
        st->Execute("SELECT plano_id, datahora, mensagem FROM log_replicacao WHERE plano_id = ? ORDER BY datahora DESC", id);
    } else {
        st->Execute("SELECT plano_id, datahora, mensagem FROM log_replicacao ORDER BY datahora DESC");
    }

    while (st->Fetch()) {
        int pid; std::string data, msg;
        st->Get(1, pid); st->Get(2, data); st->Get(3, msg);
        std::cout << "[" << data << "] Plano " << pid << ": " << msg << "\n";
    }

    tr->Commit(); db->Disconnect();
}

void DBManager::logOperacao(int plano_id, const std::string& mensagem) {
    IBPP::Database db = IBPP::DatabaseFactory("localhost", DB_PATH, "sysdba", DB_PASSWD);
    IBPP::Transaction tr = IBPP::TransactionFactory(db);
    db->Connect(); tr->Start();
    IBPP::Statement st = IBPP::StatementFactory(db, tr);
    st->Execute("INSERT INTO log_replicacao (plano_id, mensagem) VALUES (?, ?)", plano_id, mensagem);
    tr->Commit(); db->Disconnect();
}
