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

        TEST_METHOD(TestPacketParsing)
        {
            GroundControl::GroundControl gc;
            std::string message = "Flight Number: AA123\nAircraft Type: Boeing 747\nDeparture Airport: JFK\nDestination Airport: LAX\n";
            gc.PacketParsing(message);

            // Add assertions here to verify the parsing logic
        }

        TEST_METHOD(TestGenerateChecksum)
        {
            GroundControl::GroundControl gc;
            std::string packetContent = "Flight Number: AA123\nAircraft Type: Boeing 747\nDeparture Airport: JFK\nDestination Airport: LAX\n";
            std::string checksum = gc.generateChecksum(packetContent);

            // Add assertions here to verify the checksum generation
        }

        TEST_METHOD(TestChecksumCheck)
        {
            GroundControl::GroundControl gc;
            std::string message = "Flight Number: AA123\nAircraft Type: Boeing 747\nDeparture Airport: JFK\nDestination Airport: LAX\n";
            gc.ChecksumCheck(message);

            // Add assertions here to verify the checksum check logic
        }
    };
}