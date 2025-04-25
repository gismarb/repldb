#!/bin/bash

# Caminho raiz do projeto
ROOT_DIR=$(dirname "$(realpath "$0")")/..
DATA_DIR="$ROOT_DIR/data"
DB_FILE="$DATA_DIR/repldb.fdb"

# Criação da pasta data se necessário
mkdir -p "$DATA_DIR"

# Verificação se banco já existe
if [ -f "$DB_FILE" ]; then
    echo "Banco de dados já existe em: $DB_FILE"
else
    echo "Criando banco de dados intermediário..."
    "$ROOT_DIR/bin/replicadb" --init-db
fi
