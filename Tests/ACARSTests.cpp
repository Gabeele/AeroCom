#include "pch.h"
#include "CppUnitTest.h"
#include "../client/ACARS.h" 
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aircraft; 

namespace ACARSTests
{
    TEST_CLASS(ACARSTests)
    {
    public:

        TEST_METHOD(SerializePacketCorrectly)
        {
            ACARS acars(1, true, ACARSFlag::Data, "AID123", "GID123",
                "FL123", "A320", "JFK", "LAX", 40.7128f, -74.0060f,
                30000.0f, 500.0f, 180);

            
            std::string packet = acars.serializePacket();

           
            std::string expectedStart = "START";
            std::string expectedPriority = "Message Priority: Yes";
            std::string expectedFlag = "Flag: D";
            std::string expectedAircraftID = "Aircraft ID: AID123";
            std::string expectedGroundStationID = "Ground Station ID: GID123";
            std::string expectedFlightInfo = "Flight Information:";
            std::string expectedFlightNumber = "\tFlight Number: FL123";
            std::string expectedMessageTerminator = "Message Terminator: END";

            std::string expectedTimestampFormat = "Time Stamp:"; 

            Assert::IsTrue(packet.find(expectedStart) != std::string::npos);
            Assert::IsTrue(packet.find(expectedPriority) != std::string::npos);
            Assert::IsTrue(packet.find(expectedFlag) != std::string::npos);
            Assert::IsTrue(packet.find(expectedAircraftID) != std::string::npos);
            Assert::IsTrue(packet.find(expectedGroundStationID) != std::string::npos);
            Assert::IsTrue(packet.find(expectedFlightInfo) != std::string::npos);
            Assert::IsTrue(packet.find(expectedFlightNumber) != std::string::npos);
            Assert::IsTrue(packet.find(expectedMessageTerminator) != std::string::npos);
            Assert::IsTrue(packet.find(expectedTimestampFormat) != std::string::npos);
        }

        TEST_METHOD(GenerateChecksum)
        {
            ACARS acars; 
            std::string content = "Test Content";
            std::string checksum = acars.generateChecksum(content);

            std::string expected_checksum = "49b";
            Assert::AreEqual(expected_checksum, checksum);
        }

        TEST_METHOD(SetTransmissionNumber)
        {
            ACARS acars;
            acars.setTransmissionNumber(42);


            std::string packet = acars.serializePacket();
            Assert::IsTrue(packet.find("Transmission Number: 42") != std::string::npos);
        }

        TEST_METHOD(PriorityFlag)
        {
            ACARS acars;
            acars.setIsPriority(true);

            std::string packet = acars.serializePacket();
            Assert::IsTrue(packet.find("Message Priority: Yes") != std::string::npos);
        }

        TEST_METHOD(SetTelemetryData)
        {
            ACARS acars;
            acars.setTelemetryData(40.7128f, -74.0060f, 30000.0f, 500.0f, 180);

            std::string packet = acars.serializePacket();
            Assert::IsTrue(packet.find("Latitude: 40.7128") != std::string::npos);

        }


    };
}
