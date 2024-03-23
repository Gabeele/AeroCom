#include "Logger.h"

namespace logs {

    Logger::Logger(const std::string& logFile, bool logToStdout)
        : logToStdout(logToStdout), isStreamValid(false) {
        if (logToStdout) {
            // Logging to stdout does not require initialization.
            isStreamValid = true;
        }

        if (!logFile.empty()) {
            logStream.open(logFile, std::ios::out | std::ios::app);
            isStreamValid = logStream.is_open();
            if (!isStreamValid) {
                // Handle the case where the file could not be opened.
                // Logging to a file is not possible, but stdout logging can continue if enabled.
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
        bool exitFlag = false;
        std::time_t t = std::time(nullptr);
        std::tm tm;
        errno_t localtimeResult = localtime_s(&tm, &t); 

        if (localtimeResult != 0) {
            exitFlag = true;
        }

        if (!exitFlag) {
            std::ostringstream logStreamMsg;
            logStreamMsg << "[" << tm.tm_year + 1900 << "-"
                << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << "-"
                << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
                << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
                << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
                << std::setfill('0') << std::setw(2) << tm.tm_sec << "] "
                << "[" << logLevelToString(level) << "] " << message;

            // Convert the stringstream to std::string
            std::string logMessage = logStreamMsg.str();

            // Write to file if stream is valid
            if (isStreamValid) {
                logStream << logMessage << std::endl;
            }

            // Optionally, write to stdout
            if (logToStdout) {
                std::cout << logMessage << std::endl;
            }
        
        }
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

   Logger logger("log.txt", true);
}