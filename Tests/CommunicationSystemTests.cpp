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
        TEST_METHOD(TestGenerateChecksum)
        {
            GroundControl::GroundControl gc;
            std::string message = "Flight Number: AA123\nAircraft Type: Boeing 747\nDeparture Airport: JFK\nDestination Airport: LAX\nChecksum: 1f83\n";

            std::string actual = gc.generateChecksum(message);
            std::string expected = "1f83";

            // Add assertions here to verify the checksum generation
            Assert::AreEqual(expected, actual);
        }
    };
}