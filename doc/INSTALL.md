# Guia de Instalação

## Pré-requisitos
- Linux (testado em Ubuntu Server 20.04+)
- Firebird 3.0 instalado em `/opt/firebird` - e funcionando adequadamente
- GCC/G++ e Make
- Criação das pasta `/opt/firebird/data`

## Passos
1. Clone o repositório:
   ```bash
   git clone https://github.com/gismarb/repldb.git
   cd repldb
   ```

2. Compile e/ou instale:
   ```bash
   make clean all
   ```

3. Inicie o banco de dados de replicação (`repldb.fdb`)

   ```bash
   sudo ./bin/repldb --init-db
   ```

4. Configure permissões e paths do Firebird, se necessário.

   ```bash
   sudo chown firebird:firebird /opt/firebird/bin/repldb
   sudo chown firebird:firebird /opt/firebird/data/repldb.fdb
   ```
