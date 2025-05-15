#!/bin/bash

# PENDENTE REFATORACAO (LINGUA)

ID=$1

if [[ -z "$ID" ]]; then
  echo "Uso: $0 <id_plano>"
  exit 1
fi

TMP="/tmp/cron_repldb_rm_$ID"
crontab -l 2>/dev/null > "$TMP"

# Remove linha com marcador e comando imediatamente após
sed -i "/# REPLDB_JOB_$ID/,+1d" "$TMP"

crontab "$TMP"
rm "$TMP"

echo "Tarefa $ID removida do agendamento."

