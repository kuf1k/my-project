#ifndef DATEUTILS_H
#define DATEUTILS_H
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace core {
    enum class FreshnessStatus {
        Fresh,
        ExpiringSoon,
        Expired
    };

    class Dateutils {
    public:
        static int getDaysUntil(const std::string &expirationDate, const std::string &currentDate) {
            auto parseDate = [](const std::string &dateStr) {
                std::tm tm{};
                std::istringstream streamDate(dateStr);
                streamDate >> std::get_time(&tm, "%Y-%m-%d");
                return std::chrono::system_clock::from_time_t(std::mktime(&tm));
            };
            auto expTime = parseDate(expirationDate);
            auto currTime = parseDate(currentDate);
            auto duration = std::chrono::duration_cast<std::chrono::hours>(expTime - currTime);
            return static_cast<int>(duration.count() / 24);
        }

        static FreshnessStatus calculateStatus(const std::string &expirationDate, const std::string &currentDate) {
            int daysLeft = getDaysUntil(expirationDate, currentDate);
            if (daysLeft < 0) {
                return FreshnessStatus::Expired;
            } else if (daysLeft == 0) {
                return FreshnessStatus::ExpiringSoon;
            }
            return FreshnessStatus::Fresh;
        }

        static std::string getStatusLabel(FreshnessStatus status) {
            switch (status) {
                case FreshnessStatus::Fresh:
                    return "[Fresh]";
                case FreshnessStatus::ExpiringSoon:
                    return "[Expiring Soon]";
                case FreshnessStatus::Expired:
                    return "[Expired]";
            }
            return "The status is not avaliable";
        }
    };
}


#endif //DATEUTILS_H
