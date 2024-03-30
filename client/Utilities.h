#pragma once

#include <string>

namespace aircraft {


    const std::string DEFAULT_FREQUENCY = "127.0.0.1";
    const std::string DEFAULT_CHANNEL_STR = "VHF";
    const std::string TRAJECTORY_PATH = "./trajectory.png";

    enum class AircraftState { Idle, InFlight, Takeoff, Landing, Completed };
    enum class SystemState { ON, OFF };
    enum class FlightType { Cargo, Commercial, Private };
    enum class CommunicationState { Closed, EstablishedConnection, Sending, Handoff };
    enum class ACARSFlag { Handoff, Data, Message, Acknowledge, Request };
    enum class CommunicationType : std::uint16_t { VHF = 5555, HF = 4444 };


    // Function declarations
    std::string aircraftStateToString(AircraftState state);
    AircraftState fromStringToAircraftState(const std::string& stateStr);
    std::string SystemStateToString(SystemState state);
    SystemState fromStringToSystemState(const std::string& stateStr);
    std::string FlightTypeToString(FlightType type);
    FlightType fromStringToFlightType(const std::string& typeStr);
    std::string CommunicationStateToString(CommunicationState state);
    CommunicationState fromStringToCommunicationState(const std::string& stateStr);
    std::string AcarsFlagToString(ACARSFlag flag);
    ACARSFlag fromStringToAcarsFlag(const std::string& flagStr);
    std::string CommunicationTypeToString(CommunicationType type);
    CommunicationType fromStringToCommunicationType(const std::string& typeStr);
}
