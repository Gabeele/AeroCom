// This file is used to denote the headers for logger

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

        /// <summary>
        /// Create a log object
        /// </summary>
        /// <param name="logFile">Path and name of the log file.</param>
        /// <param name="logToStdout">Set to true if you wish to print to the screen.</param>
        Logger(const std::string& logFile, bool logToStdout);

        ~Logger();

        /// <summary>
        /// Write to the log object
        /// </summary>
        /// <param name="message">A string message of the log.</param>
        /// <param name="level">The level for the log, ie. Error, Info, Warning.</param>
        void log(const std::string& message, LogLevel level);


    private:
        std::ofstream logStream;    // The log stream
        bool logToStdout;           // True if logging to the screen
        bool isStreamValid;         // The file did open correctly

        /// <summary>
        /// Get a formated time string
        /// </summary>
        /// <returns>A formated time string.</returns>
        std::string getTimeString() const;

        /// <summary>
        /// Convert the log level to a string
        /// </summary>
        /// <param name="level">The log level as the enum.</param>
        /// <returns>A string of the log level.</returns>
        std::string logLevelToString(LogLevel level) const;
    };

    /// <summary>
    /// A global logger object to use in all the files. 
    /// </summary>
    extern Logger logger;


}
#endif 