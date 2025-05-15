#!/bin/bash

# PENDENTE REFATORACAO (LINGUA)

ID=$1
CRON_EXPR=$2
COMANDO=$3

if [[ -z "$ID" || -z "$CRON_EXPR" || -z "$COMANDO" ]]; then
  echo "Uso: $0 <id_plano> '<cron_expr>' '<comando>'"
  exit 1
fi

TMP="/tmp/cron_repldb_$ID"
crontab -l 2>/dev/null > "$TMP"

# Remove entradas duplicadas com o mesmo marcador
sed -i "/# REPLDB_JOB_$ID/d" "$TMP"

# Adiciona nova entrada com marcação
echo "# REPLDB_JOB_$ID" >> "$TMP"
echo "$CRON_EXPR $COMANDO" >> "$TMP"

crontab "$TMP"
rm "$TMP"

echo "Tarefa $ID agendada com sucesso."

