#include "CronHelper.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "Utils.h" // para uso do trim()

// PENDENTE REFATORACAO (LINGUA)

const std::string CRON_MARKER_PREFIX = "# REPLDB_JOB_";

bool CronHelper::addCronJob(const std::string& schedule, const std::string& command, const std::string& jobId) {
    std::string marker = CRON_MARKER_PREFIX + jobId;

    // Salva crontab atual
    std::system("crontab -l > /tmp/current_cron 2>/dev/null");

    std::ifstream in("/tmp/current_cron");
    std::ofstream out("/tmp/new_cron");

    std::string line;
    while (std::getline(in, line)) {
        if (line.find(marker) == std::string::npos) {
            out << line << "\n";
        }
    }
    in.close();

    out << marker << "\n";
    out << schedule << " " << command << "\n";
    out.close();

    int result = std::system("crontab /tmp/new_cron");
    std::remove("/tmp/current_cron");
    std::remove("/tmp/new_cron");

    return result == 0;
}

bool CronHelper::removeCronJob(const std::string& execPath, const std::string& jobId) {
    std::string cmd = "crontab -l > /tmp/current_cron.txt 2>/dev/null";
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "[repldb] Aviso: crontab vazio ou não existente.\n";
        return false;
    }

    std::ifstream infile("/tmp/current_cron.txt");
    std::ofstream outfile("/tmp/updated_cron.txt");

    if (!infile || !outfile) {
        std::cerr << "[repldb] Falha ao abrir arquivos temporários para remoção do cron.\n";
        return false;
    }

    std::string linha;
    std::string cronCommand = execPath + " --run-replica --id " + jobId;
    std::string marker = "# REPLDB_JOB_" + jobId;
    bool found = false;
    bool skipNext = false;

    while (std::getline(infile, linha)) {
        std::string trimmed = Utils::trim(linha);

        // Permite encontrar o comando mesmo que ele tenha redirecionamento ou espaços extras
        if (trimmed.find(cronCommand) != std::string::npos) {
            found = true;
            skipNext = true; // pula esta e a próxima (comentário)
            continue;
        }

        if (trimmed == marker) {
            if (!found) {
                std::cerr << "[repldb] Aviso: marcador encontrado sem comando correspondente.\n";
            }
            continue;
        }

        if (skipNext) {
            skipNext = false;
            continue;
        }

        outfile << linha << "\n";
    }

    infile.close();
    outfile.close();

    std::string applyCmd = "crontab /tmp/updated_cron.txt";
    std::system(applyCmd.c_str());

    std::remove("/tmp/current_cron.txt");
    std::remove("/tmp/updated_cron.txt");

    return found;
}

bool CronHelper::cronJobExists(const std::string& jobId) {
    std::string marker = CRON_MARKER_PREFIX + jobId;
    std::ostringstream cmd;
    cmd << "crontab -l 2>/dev/null | grep \"" << marker << "\" > /dev/null";
    return std::system(cmd.str().c_str()) == 0;
}

