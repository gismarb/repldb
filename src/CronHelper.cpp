#include "CronHelper.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

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

bool CronHelper::removeCronJob(const std::string& jobId) {
    std::string marker = CRON_MARKER_PREFIX + jobId;
    std::system("crontab -l > /tmp/current_cron 2>/dev/null");

    std::ifstream in("/tmp/current_cron");
    std::ofstream out("/tmp/new_cron");

    std::string line;
    bool skipNext = false;
    while (std::getline(in, line)) {
        if (line.find(marker) != std::string::npos) {
            skipNext = true;
            continue;
        }
        if (skipNext) {
            skipNext = false;
            continue;
        }
        out << line << "\n";
    }

    in.close();
    out.close();

    int result = std::system("crontab /tmp/new_cron");
    std::remove("/tmp/current_cron");
    std::remove("/tmp/new_cron");

    return result == 0;
}

bool CronHelper::cronJobExists(const std::string& jobId) {
    std::string marker = CRON_MARKER_PREFIX + jobId;
    std::ostringstream cmd;
    cmd << "crontab -l 2>/dev/null | grep \"" << marker << "\" > /dev/null";
    return std::system(cmd.str().c_str()) == 0;
}

