#ifndef CRON_HELPER_H
#define CRON_HELPER_H

#include <string>

class CronHelper {
public:
    static bool addCronJob(const std::string& schedule, const std::string& command, const std::string& jobId);
    static bool removeCronJob(const std::string& jobId);
    static bool cronJobExists(const std::string& jobId);
};

#endif // CRON_HELPER_H

