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

        Logger(const std::string& logFile, bool logToStdout);

        ~Logger();

        void log(const std::string& message, LogLevel level);

    private:
        std::ofstream logStream; 
        bool logToStdout;        
        bool isStreamValid;      

        std::string logLevelToString(LogLevel level) const;
    };

    extern Logger logger;


#endif 
}