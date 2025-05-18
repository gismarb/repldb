# repldb

***repldb** é uma ferramenta de linha de comando desenvolvida em C++ para gerenciar replicações de bancos de dados Firebird 3.0 via backup/restore. Ela suporta replicações locais e remotas, controle via banco intermediário (`repldb.fdb`) e agendamento via `cron`.*

---



## Requisitos

- Linux (server-side)
- Firebird 3.0 instalado em `/opt/firebird`
- Compilador C++17
- Permissões para editar o `crontab`

---



## Compilação

```bash
make
```



## Instalação

```bash
sudo make install
```



## Uso

```bash
repldb --init-db
repldb --add-replica --fonte <path> --destino <path> [--schedule "<cron>"]
repldb --remove-replica --id <id>
repldb --list-replica
repldb --run-replica --id <id>
repldb --run-replica --all
repldb --list-logs [--id <id>]
repldb --help
```



## Estrutura do Banco de Controle (`repldb.fdb`)

Tabela `planos_replicacao`: registros de replicações planejadas

Tabela `log_replicacao`: registros de execuções realizadas

Suporte para cron via marcador `# REPLDB_JOB_<id>`



## Exemplos de Agendamentos

```bash
# Agendamento local
repldb --add-replica --fonte /opt/firebird/data/master.fdb --destino /opt/firebird/data/replica.fdb --schedule "0 2 * * *"

# Agendamento remoto
repldb --add-replica --fonte /opt/firebird/data/master.fdb --destino 192.168.1.54:/opt/firebird/data/replica.fdb --schedule "0 2 * * *"
```



## Execução Manual

```bash
# Execução manual de um único plano selecionado
repldb --run-replica --id 1

# Execução manual de todos os planos cadastrados
repldb --run-replica --all
```



## Remover Plano

```bash
repldb --remove-replica --id 1
```



## Estrutura de Diretórios

```bash
repldb
├── db                      	# Script SQL (estrutura do banco de dados de replicação)
│   └── init.sql
├── doc                     	# Documentos de referência para o projeto 
│   ├── man.txt
│   ├── title.txt
│   ├── TODO.txt
│   ├── tree_d.txt
│   └── tree.txt
├── include                     # Arquivos de cabeçalho (header files)
│   ├── ArgumentParser.h
│   ├── CronHelper.h
│   ├── DBHelper.h
│   ├── DBManager.h
│   ├── Replicator.h
│   └── Utils.h
├── LICENSE                     # Temos de licenciamento do uso do conteúdo deste projeto
├── Makefile                    # Makefile com instruções para compilar, executar, reconstruir e instalar projeto
├── README.md                   # Mini documentação sobre o projeto
├── scripts                     # Scripts bash para auxílio nas transações do projeto
│   ├── cron_add.sh
│   └── cron_remove.sh
└── src                     	# Módulos do progama (implementações)
    ├── ArgumentParser.cpp
    ├── CronHelper.cpp
    ├── DBHelper.cpp
    ├── DBManager.cpp
    ├── main.cpp
    ├── Replicator.cpp
    └── Utils.cpp


```



## Observações

*A base `repldb.fdb` não é replicada.*

*Os caminhos de banco (fonte/destino) devem ser válidos e acessíveis.*

*Todos os hosts devem usar Firebird 3.0, com mesma estrutura em `/opt/firebird`.*
