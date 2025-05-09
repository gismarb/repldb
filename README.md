# repldb

**repldb** é uma ferramenta de linha de comando desenvolvida em C++ para gerenciar replicações de bancos de dados Firebird 3.0 via backup/restore. Ela suporta replicações locais e remotas, controle via banco intermediário (`repldb.fdb`) e agendamento via `cron`.

---

## 📦 Requisitos

- Linux (server-side)
- Firebird 3.0 instalado em `/opt/firebird`
- Compilador C++17
- Permissões para editar o `crontab`

---

## 🛠️ Compilação

```bash
make


Instalacao
sudo make install

🚀 Instalação
bash
Copy
sudo make install

🔧 Uso
bash
Copy
repldb --init-db
repldb --add-replica --fonte <path> --destino <path> [--schedule "<cron>"]
repldb --remove-replica --id <id>
repldb --list-replica
repldb --run-replica --id <id>
repldb --run-replica --all
repldb --list-logs [--id <id>]

🗃️ Estrutura do Banco de Controle (repldb.fdb)
Tabela planos_replicacao: registros de replicações planejadas

Tabela log_replicacao: registros de execuções realizadas

Suporte para cron via marcador # REPLDB_JOB_<id>


📑 Exemplo de Agendamento
bash
Copy
repldb --add-replica --fonte /opt/firebird/data/master.fdb --destino /opt/firebird/data/replica.fdb --schedule "0 2 * * *"


🔁 Execução Manual
bash
Copy
repldb --run-replica --id 1


🧹 Remover Plano
bash
Copy
repldb --remove-replica --id 1


📂 Estrutura de Diretórios
makefile
Copy
include/        # Arquivos .h
src/            # Arquivos .cpp
db/             # Script SQL de estrutura
scripts/        # Scripts auxiliares (cron)
Makefile        # Compilação e instalação


📌 Observações
A base repldb.fdb não é replicada.

Os caminhos de banco (fonte/destino) devem ser válidos e acessíveis.

Todos os hosts devem usar Firebird 3.0, com mesma estrutura em /opt/firebird.



