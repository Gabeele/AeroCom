// This file is used to denote the functions for logger

#include "logger.h"

namespace logs {

    Logger::Logger(const std::string& logFile, bool logToStdout)
        : logToStdout(logToStdout), isStreamValid(false) {

        if (logToStdout) {
            isStreamValid = true;
        }

        if (!logFile.empty()) {
            logStream.open(logFile, std::ios::out | std::ios::app);
            isStreamValid = logStream.is_open();
            if (!isStreamValid) {
                std::cerr << "Unable to open log file: " << logFile << std::endl;
            }
        }
    }

    Logger::~Logger() {
        if (logStream.is_open()) {
            logStream.close();
        }
    }

    void Logger::log(const std::string& message, LogLevel level) {
        std::string timeStr = getTimeString();
        std::ostringstream logStreamMsg;

        logStreamMsg << timeStr << " "
            << "[" << logLevelToString(level) << "] " << message;

        std::string logMessage = logStreamMsg.str();

        // Write the streams if valid 
        if (isStreamValid) {
            logStream << logMessage << std::endl;
        }

        if (logToStdout) {
            std::cout << logMessage << std::endl;
        }
    }

    std::string Logger::getTimeString() const {
        std::time_t t = std::time(nullptr);
        std::tm tm;
        errno_t localtimeResult = localtime_s(&tm, &t);

        std::string result;

        /// Use the current time and build a time stamp string
        if (localtimeResult != 0) {
            result = "[Error getting time]";
        }
        else {

            std::ostringstream timestampStream;
            timestampStream << "[" << tm.tm_year + 1900 << "-"
                << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << "-"
                << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
                << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
                << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
                << std::setfill('0') << std::setw(2) << tm.tm_sec << "]";
            result = timestampStream.str();
        }

        return result;
    }

    std::string Logger::logLevelToString(LogLevel level) const {
        std::string levelString;

        switch (level) {
        case LogLevel::Info:
            levelString = "INFO";
            break;
        case LogLevel::Warning:
            levelString = "WARNING";
            break;
        case LogLevel::Error:
            levelString = "ERROR";
            break;
        default:
            levelString = "UNKNOWN";
            break;
        }
        return levelString;
    }

    Logger logger("log.txt", true); // Globally scoped logging functionality
}