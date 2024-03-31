#include "pch.h"
#include "CppUnitTest.h"
#include "../client/Utilities.h" 
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aircraft;

namespace UtilitiesTests
{
    TEST_CLASS(UtilitiesTests)
    {
    public:

        // AircraftState Tests
        TEST_METHOD(AircraftStateToString)
        {
            Assert::AreEqual(std::string("Idle"), aircraftStateToString(AircraftState::Idle));
            Assert::AreEqual(std::string("InFlight"), aircraftStateToString(AircraftState::InFlight));
            Assert::AreEqual(std::string("Takeoff"), aircraftStateToString(AircraftState::Takeoff));
            Assert::AreEqual(std::string("Landing"), aircraftStateToString(AircraftState::Landing));
            Assert::AreEqual(std::string("Completed"), aircraftStateToString(AircraftState::Completed));
        }

        TEST_METHOD(FromStringToAircraftState)
        {
            Assert::IsTrue(AircraftState::Idle == fromStringToAircraftState("Idle"));
            Assert::IsTrue(AircraftState::InFlight == fromStringToAircraftState("InFlight"));
            Assert::IsTrue(AircraftState::Takeoff == fromStringToAircraftState("Takeoff"));
            Assert::IsTrue(AircraftState::Landing == fromStringToAircraftState("Landing"));
            Assert::IsTrue(AircraftState::Completed == fromStringToAircraftState("Completed"));
        }

        // SystemState Tests
        TEST_METHOD(SystemStateString)
        {
            Assert::AreEqual(std::string("ON"), SystemStateToString(SystemState::ON));
            Assert::AreEqual(std::string("OFF"), SystemStateToString(SystemState::OFF));
        }

        TEST_METHOD(FromStringToSystemState)
        {
            Assert::IsTrue(SystemState::ON == fromStringToSystemState("ON"));
            Assert::IsTrue(SystemState::OFF == fromStringToSystemState("OFF"));
        }

        // FlightType Tests
        TEST_METHOD(FlightTypeString)
        {
            Assert::AreEqual(std::string("Cargo"), FlightTypeToString(FlightType::Cargo));
            Assert::AreEqual(std::string("Commercial"), FlightTypeToString(FlightType::Commercial));
            Assert::AreEqual(std::string("Private"), FlightTypeToString(FlightType::Private));
        }

        TEST_METHOD(FromStringToFlightType)
        {
            Assert::IsTrue(FlightType::Cargo == fromStringToFlightType("Cargo"));
            Assert::IsTrue(FlightType::Commercial == fromStringToFlightType("Commercial"));
            Assert::IsTrue(FlightType::Private == fromStringToFlightType("Private"));
        }

        // CommunicationState Tests
        TEST_METHOD(CommunicationStateString)
        {
            Assert::AreEqual(std::string("Closed"), CommunicationStateToString(CommunicationState::Closed));
            Assert::AreEqual(std::string("EstablishedConnection"), CommunicationStateToString(CommunicationState::EstablishedConnection));
            Assert::AreEqual(std::string("Sending"), CommunicationStateToString(CommunicationState::Sending));
            Assert::AreEqual(std::string("Handoff"), CommunicationStateToString(CommunicationState::Handoff));
        }

        TEST_METHOD(FromStringToCommunicationState)
        {
            Assert::IsTrue(CommunicationState::Closed == fromStringToCommunicationState("Closed"));
            Assert::IsTrue(CommunicationState::EstablishedConnection == fromStringToCommunicationState("EstablishedConnection"));
            Assert::IsTrue(CommunicationState::Sending == fromStringToCommunicationState("Sending"));
            Assert::IsTrue(CommunicationState::Handoff == fromStringToCommunicationState("Handoff"));
        }

        // ACARSFlag Tests
        TEST_METHOD(AcarsFlagString)
        {
            Assert::AreEqual(std::string("Handoff"), AcarsFlagToString(ACARSFlag::Handoff));
            Assert::AreEqual(std::string("Data"), AcarsFlagToString(ACARSFlag::Data));
            Assert::AreEqual(std::string("Message"), AcarsFlagToString(ACARSFlag::Message));
            Assert::AreEqual(std::string("Acknowledge"), AcarsFlagToString(ACARSFlag::Acknowledge));
            Assert::AreEqual(std::string("Request"), AcarsFlagToString(ACARSFlag::Request));
        }

        TEST_METHOD(FromStringToAcarsFlag)
        {
            Assert::IsTrue(ACARSFlag::Handoff == fromStringToAcarsFlag("Handoff"));
            Assert::IsTrue(ACARSFlag::Data == fromStringToAcarsFlag("Data"));
            Assert::IsTrue(ACARSFlag::Message == fromStringToAcarsFlag("Message"));
            Assert::IsTrue(ACARSFlag::Acknowledge == fromStringToAcarsFlag("Acknowledge"));
            Assert::IsTrue(ACARSFlag::Request == fromStringToAcarsFlag("Request"));
        }

        // CommunicationType Tests
        TEST_METHOD(CommunicationTypeString)
        {
            Assert::AreEqual(std::string("VHF"), CommunicationTypeToString(CommunicationType::VHF));
            Assert::AreEqual(std::string("HF"), CommunicationTypeToString(CommunicationType::HF));
        }

        TEST_METHOD(FromStringToCommunicationType)
        {
            Assert::IsTrue(CommunicationType::VHF == fromStringToCommunicationType("VHF"));
            Assert::IsTrue(CommunicationType::HF == fromStringToCommunicationType("HF"));
        }
    };
}