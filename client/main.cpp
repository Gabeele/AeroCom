#include <iostream>
#include "logger.h"

int main()
{
	std::cout << "hello world from the client";

    logs::Logger logger("log.txt", true); // true means log to stdout as well

    // Test logging with different levels and messages
    logger.log("This is an informational message.", logs::Logger::LogLevel::Info);
    logger.log("This is a warning message.", logs::Logger::LogLevel::Warning);
    logger.log("This is an error message.", logs::Logger::LogLevel::Error);


	return 0;
}
