# Guia de Uso

## Executando o ReplDB

### Ajuda Geral
```bash
sudo ./bin/repldb --help
#    Uso:
#        repldb --init-db
#        repldb --add-replica --fonte <path> --destino <path> [--schedule "<cron>"]
#        repldb --remove-replica --id <id>
#        repldb --list-replica
#        repldb --run-replica --id <id>
#        repldb --run-replica --all
#        repldb --list-logs [--id <id>]
#        repldb --help
```

### Adicionar Réplica
```bash
# Replica local
sudo ./bin/repldb --add-replica --fonte /opt/firebird/data/bd_master_mini_erp.fdb --destino /opt/firebird/data/bd_replica01_mini_erp.fdb

# Replica remota
sudo ./bin/repldb --add-replica --fonte /opt/firebird/data/bd_master_mini_erp.fdb --destino 192.168.1.54:/opt/firebird/data/bd_replica02_mini_erp.fdb --schedule "*/3 * * * *"
```

### Listar Planos de Replicação
```bash
sudo ./bin/repldb --list-replica

# Listando tarefa criada no Cron (apenas verificação)
sudo crontab -l
```

### Execute os Planos de Replicação

```bash
sudo ./bin/repldb --run-replica --id 1
sudo ./bin/repldb --run-replica --id 2
```

### Listar Logs de Replicação

```bash
sudo ./bin/repldb --list-logs
```

### Excluindo Planos de Replicação

```bash
sudo ./bin/repldb --remove-replica --id 1
sudo ./bin/repldb --remove-replica --id 2
```
