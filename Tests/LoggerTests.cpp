#include "pch.h"
#include "CppUnitTest.h"
#include "../client/logger.h" 
#include <fstream>
#include <sstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace LoggerUnitTests
{
	TEST_CLASS(LoggerUnitTests)
	{
	public:

		TEST_METHOD(InitializeWithValidFilePath)
		{
			std::string validLogPath = "validLogTest.log";
			logs::Logger logger(validLogPath, false);
			std::ifstream logFile(validLogPath);
			bool fileExists = logFile.is_open();
			logFile.close(); // Close before potentially deleting

			// Assert file was created/opened
			Assert::IsTrue(fileExists, L"Logger did not create or open the specified log file correctly.");

			// Cleanup
			if (fileExists) {
				remove(validLogPath.c_str());
			}
		}


		TEST_METHOD(LogInfoLevelMessageToFile)
		{

			std::string logPath = "../../client/logInfoTest.txt";
			logs::Logger logger(logPath, false);
			logs::logger.log("Test INFO message", logs::Logger::LogLevel::Info);

			std::ifstream logFile(logPath);
			std::string loggedContent((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
			logFile.close();

			// Assert that the logged content contains the message
			Assert::IsTrue(loggedContent.find("Test INFO message") != std::string::npos, L"INFO level message was not logged correctly.");

			// Cleanup
			//remove(logPath.c_str());
		}
	};
}