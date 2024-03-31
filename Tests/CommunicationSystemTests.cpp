#include "pch.h"
#include "CppUnitTest.h"
#include "../server/communication_system.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GroundControlTests
{
    TEST_CLASS(GroundControlTests)
    {
    public:

        TEST_METHOD(Initialize)
        {
            // Initialize Winsock
            GroundControl::GroundControl gc;
            Assert::IsTrue(gc.Initialize());
        }

        TEST_METHOD(Cleanup)
        {
            // Cleanup Winsock
            GroundControl::GroundControl gc;
            gc.~GroundControl();
        }

        TEST_METHOD(TestConnect)
        {
            GroundControl::GroundControl gc;
            gc.Initialize();
            Assert::IsTrue(gc.Connect(4444));
        }

        TEST_METHOD(TestSwitchFrequency)
        {
            GroundControl::GroundControl gc;
            Assert::IsTrue(gc.SwitchFrequency(5678));
        }

        TEST_METHOD(TestAcceptConnection)
        {
            GroundControl::GroundControl gc;
            gc.Connect(1234);

            SOCKET clientSocket = gc.AcceptConnection();

            Assert::IsNotNull(&clientSocket);
        }

        TEST_METHOD(TestReceiveMessage)
        {
            // Assuming there's a connected client
            GroundControl::GroundControl gc;
            gc.Connect(4444);
            SOCKET clientSocket = gc.AcceptConnection();

            // Simulate receiving a message
            std::string message = "Test message";
            send(clientSocket, message.c_str(), message.length(), 0);

            Assert::AreEqual(message, gc.ReceiveMessage(clientSocket));
        }

        TEST_METHOD(TestChecksumCheck)
        {

        }
    };
}