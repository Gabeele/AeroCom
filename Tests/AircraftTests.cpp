#include "pch.h"
#include "CppUnitTest.h"
#include "../client/aircraft.h" 
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aircraft;

namespace Aircraft_Tests
{

    // Need to make some of the properties in aircraft publically accessible. 
    TEST_CLASS(AircraftTests)
    {
    public:

        //TEST_METHOD(Aircraft_Constructor_Defaults)
        //{
        //    aircraft::Aircraft testAircraft("TestAircraft123");

        //    Assert::AreEqual(std::string("TestAircraft123"), testAircraft.identifier, L"Identifier mismatch");
        //    Assert::AreEqual(aircraft::AircraftState::InFlight, testAircraft.getAircraftState(), L"Initial state should be InFlight");
        //}


        //TEST_METHOD(Aircraft_UpdateState)
        //{
        //    aircraft::Aircraft testAircraft("TestUpdateState");

        //    testAircraft.updateAircraftState(aircraft::AircraftState::Landing);
        //    Assert::AreEqual(aircraft::AircraftState::Landing, testAircraft.getAircraftState(), L"Aircraft state wasn't updated correctly");
        //}

        //TEST_METHOD(Aircraft_ToggleComms)
        //{
        //    aircraft::Aircraft testAircraft("TestToggleComms");

        //    // Initial state OFF assumed
        //    testAircraft.toggleCommunicationSystem();
        //    Assert::IsTrue(testAircraft.commsToggle == aircraft::SystemState::ON, L"Comms should be ON after toggle");

        //    testAircraft.toggleCommunicationSystem();
        //    Assert::IsTrue(testAircraft.commsToggle == aircraft::SystemState::OFF, L"Comms should be OFF after second toggle");
        //}

        //TEST_METHOD(Aircraft_ToggleACARS)
        //{
        //    aircraft::Aircraft testAircraft("TestToggleACARS");

        //    // Assuming initial state is OFF
        //    testAircraft.toggleACARSSystem();
        //    Assert::IsTrue(testAircraft.acarsToggle == aircraft::SystemState::ON, L"ACARS should be ON after toggle");

        //    testAircraft.toggleACARSSystem();
        //    Assert::IsTrue(testAircraft.acarsToggle == aircraft::SystemState::OFF, L"ACARS should be OFF after second toggle");
        //}

        //TEST_METHOD(Aircraft_ToggleSimulateTelemetry)
        //{
        //    aircraft::Aircraft testAircraft("TestSimulateTelemetry");

        //    // Trigger the simulation to start
        //    testAircraft.toggleSimulateTelemetry();
        //    Assert::IsTrue(testAircraft.simulateTelemetryActive, L"Telemetry simulation should be active after toggling ON");

        //    // Trigger the simulation to stop
        //    testAircraft.toggleSimulateTelemetry();
        //    Assert::IsFalse(testAircraft.simulateTelemetryActive, L"Telemetry simulation should not be active after toggling OFF");
        //}

        //TEST_METHOD(Aircraft_SetFrequencyChannel)
        //{
        //    aircraft::Aircraft testAircraft("TestFrequencyChannel");
        //    std::string testFrequency = "123.45";
        //    std::string testChannel = "Channel1";

        //    testAircraft.setFrequencyChannel(testFrequency, testChannel);

        //    // Assuming `getFrequency` and `getChannel` methods exist for validation
        //    Assert::AreEqual(testFrequency, testAircraft.comms.freqeuncy, L"Frequency was not set correctly");
        //    Assert::AreEqual(testChannel, testAircraft.comms.channel(), L"Channel was not set correctly");
        //}

        //TEST_METHOD(Aircraft_FinalStateAfterSimulation)
        //{
        //    aircraft::Aircraft testAircraft("TestFinalState");

        //    // Start the simulation
        //    testAircraft.toggleSimulateTelemetry();
        //    // Wait or fast-forward simulation time here...
        //    // Stop the simulation to trigger final state evaluation
        //    testAircraft.toggleSimulateTelemetry();

        //    Assert::AreEqual(aircraft::AircraftState::Completed, testAircraft.getAircraftState(), L"Aircraft should be in a Completed state after simulation");
        //}

    };
}
