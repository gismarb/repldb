#!/bin/bash

# Caminho raiz do projeto
ROOT_DIR=$(dirname "$(realpath "$0")")/..

echo "Compilando o projeto repldb..."

cd "$ROOT_DIR" || exit 1

make rebuild

if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso."
else
    echo "Erro durante a compilação."
    exit 1
fi
