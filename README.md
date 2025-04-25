# Replicador Firebird CLI

Um utilitário de linha de comando em C++ para gerenciar e executar replicações de banco de dados Firebird, voltado para ambientes Linux.

## Estrutura

O projeto é composto por scripts de inicialização, código-fonte C++ e scripts SQL:

```bash
replicador-firebird/ 
├── bin/ 
│ └── replicadb # Binário final após compilação 
├── build/ # Arquivos intermediários de build 
├── db/ 
│ └── init.sql # Script de criação das tabelas 
├── data/ 
│ └── replicadb.fdb # Banco intermediário de controle 
├── include/ 
│ ├── DBManager.h 
│ └── Utils.h 
├── src/ 
│ ├── main.cpp 
│ ├── DBManager.cpp 
│ └── Utils.cpp 
├── scripts/ 
│ └── build.sh # Script de build (Linux) 
├── Makefile 
└── README.md
```


## Pré-requisitos

- Linux
- [Firebird](https://firebirdsql.org) instalado e rodando
- [IBPP](http://www.ibpp.org/) (biblioteca C++ para Firebird)
- `g++` com suporte a C++11 ou superior

## Build

Execute:

```bash
./scripts/build.sh
