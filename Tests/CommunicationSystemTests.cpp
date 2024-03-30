#include "pch.h"
#include "CppUnitTest.h"
#include "../server/communication_system.h"
#include "../server/logger.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace GroundControl;

namespace GroundControlTests
{
    TEST_CLASS(GroundControlTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Setup)
        {
            // Initialize Winsock
            Assert::IsTrue(groundControl.Initialize());
        }

        TEST_METHOD_CLEANUP(TearDown)
        {
            // Cleanup
        }

        // Test method for Initialize
        TEST_METHOD(Initialize)
        {
            // Initialization should succeed
            Assert::IsTrue(groundControl.Initialize());
        }

        // Test method for Connect
        TEST_METHOD(Connect)
        {
            // Connection should succeed on a valid port
            Assert::IsTrue(groundControl.Connect(1234));
        }

        // Test method for SwitchFrequency
        TEST_METHOD(SwitchFrequency)
        {
            // Switching frequency should succeed on a valid new port
            Assert::IsTrue(groundControl.SwitchFrequency(5678));
        }

        // Test method for AcceptConnection
        TEST_METHOD(AcceptConnection)
        {
            // First, establish a connection (assuming port 1234 is valid)
            groundControl.Connect(1234);
            // Accepting connection should return a valid socket
            SOCKET clientSocket = groundControl.AcceptConnection();
            Assert::AreNotEqual(clientSocket, INVALID_SOCKET);
            closesocket(clientSocket); // Cleanup
        }

        // Test method for ReceiveMessage
        TEST_METHOD(ReceiveMessage)
        {
            // First, establish a connection (assuming port 1234 is valid)
            groundControl.Connect(1234);
            SOCKET clientSocket = groundControl.AcceptConnection();
            Assert::AreNotEqual(clientSocket, INVALID_SOCKET);

            // Sending a message from the client
            const std::string message = "Test message";
            send(clientSocket, message.c_str(), message.size(), 0);

            // Receiving message should return the sent message
            std::string receivedMessage = groundControl.ReceiveMessage(clientSocket);
            Assert::AreEqual(receivedMessage, message);

            closesocket(clientSocket); // Cleanup
        }

        // Test method for ChecksumCheck
        TEST_METHOD(ChecksumCheck)
        {
            // Create a valid message
            const std::string message = "Test message\nChecksum:abcd\n";
            // Checksum should match
            Assert::ExpectNoThrow(groundControl.ChecksumCheck(message));
        }

        // Test method for GenerateChecksum
        TEST_METHOD(GenerateChecksum)
        {
            // Check
            // TODO: Add appropriate test for GenerateChecksum
        }

        // Test method for HandleATCToAircraftHandoffRequest
        TEST_METHOD(HandleATCToAircraftHandoffRequest)
        {
            // Create appropriate test
            // TODO: Add appropriate test for HandleATCToAircraftHandoffRequest
        }

    };
}
