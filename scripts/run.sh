#!/bin/bash

# Caminho raiz do projeto
ROOT_DIR=$(dirname "$(realpath "$0")")/..
BIN="$ROOT_DIR/bin/repldb"

if [ ! -f "$BIN" ]; then
    echo "Executável não encontrado. Compile primeiro com build.sh."
    exit 1
fi

echo "Executando repldb com os parâmetros: $*"
"$BIN" "$@"
