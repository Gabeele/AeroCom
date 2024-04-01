#include "pch.h"
#include "CppUnitTest.h"
#include "../server/communication_system.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HandoffTests
{
    TEST_CLASS(HandoffTests)
    {
    public:

        TEST_METHOD(TestSerialization)
        {
            std::string tempFlag = "H";
            std::string tempFreq = "127.0.0.15";
            std::string tempChan = "VHF";
            GroundControl::Handoff handoff(tempFlag, tempFreq, tempChan);

            std::string expected = "Flag: H Frequency: 127.0.0.15 Channel: VHF";

            Assert::AreEqual(handoff.Serialize(), expected);
        }

    };
}