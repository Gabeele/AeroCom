#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace logs {
    class Logger {
    public:
        enum class LogLevel {
            Info,
            Warning,
            Error
        };

        // Constructor
        Logger(const std::string& logFile, bool logToStdout);

        // Destructor
        ~Logger();

        // Deleted copy constructor and assignment operator to prevent copying
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // Public logging methods
        void log(const std::string& message, LogLevel level);

    private:
        std::ofstream logStream; // File stream for logging
        bool logToStdout;        // Flag for logging to stdout
        bool isStreamValid;      // Flag indicating if the logStream is valid for writing

        // Utility method for converting log levels to string
        std::string logLevelToString(LogLevel level) const;
    };

#endif // LOGGER_H
}