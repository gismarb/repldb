# ReplDB 🚀  
**Replicação de Dados para Firebird 3.0 (Linux)**  

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
![C++](https://img.shields.io/badge/C++-17-blue)
![Firebird](https://img.shields.io/badge/Firebird-3.0-red)
![Linux](https://img.shields.io/badge/Linux-Ubuntu-orange)

---

## 📌 Sobre o Projeto
O **ReplDB** é um programa de linha de comando (CLI), desenvolvido em **C++**, para gerenciar **replicações de bases Firebird 3.0**.  
A replicação ocorre via **backup/restore** utilizando ferramentas nativas do Firebird (`gbak`, `isql`, `nbackup`), permitindo cópias **locais ou remotas** entre servidores Linux.  

O projeto foi idealizado para:
- Automatizar rotinas de replicação
- Garantir consistência de dados
- Reduzir esforços manuais de DBAs
- Minizar rotinas complexas de replicação de dados

***A Estrutura do Banco de Controle (`repldb.fdb`)*:** 

- Tabela `planos_replicacao`: registros de replicações planejadas
- Tabela `log_replicacao`: registros de execuções realizadas
- Suporte para cron via marcador `# REPLDB_JOB_<id>`

---

## ⚙️ Funcionalidades
- Replicação **local** e **remota** via Service Manager  
- Controle de planos e logs em banco intermediário (`repldb.fdb`)  
- Agendamento automático via **cron**  
- Arquitetura modular, pronta para futura expansão (triggers, replicação síncrona)  

---

## 📂 Estrutura do Repositório
```
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

---

## 🚀 Como Usar

### 1️⃣ Compilar
```bash
make clean all
```

### 2️⃣ Executar
```bash
sudo ./bin/repldb --help
```

### 3️⃣ Exemplo de uso
Replicar base local → remota:
```bash
# Agendamento local
sudo ./bin/repldb --add-replica --fonte /opt/firebird/data/master.fdb --destino /opt/firebird/data/replica.fdb --schedule "0 2 * * *"

# Agendamento remoto
sudo ./bin/repldb --add-replica --fonte /opt/firebird/data/master.fdb --destino 192.168.1.54:/opt/firebird/data/replica.fdb --schedule "0 2 * * *"
```

---

## 📖 Documentação
A documentação completa está na pasta [`doc`](./doc).  

- [Guia de Instalação](./doc/INSTALL.md)  
- [Guia de Uso](./doc/USAGE.md)  
- [Roadmap / TODO](./doc/TODO.md)  

---

## 🤝 Contribuindo
Contribuições são bem-vindas!  
Basta abrir uma [issue](https://github.com/gismarb/repldb/issues) ou enviar um pull request.  

---

## 📜 Licença
Distribuído sob a licença **GPL v3**.  
Veja [LICENSE](./LICENSE) para mais detalhes.
