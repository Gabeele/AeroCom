// This file denotes all the utility functions for enums and other global scopped artifacts
#include "Utilities.h"

namespace aircraft {

    std::string aircraftStateToString(AircraftState state) {
        if (state == AircraftState::Idle) return "Idle";
        if (state == AircraftState::InFlight) return "InFlight";
        if (state == AircraftState::Takeoff) return "Takeoff";
        if (state == AircraftState::Landing) return "Landing";
        return "Completed";
    }

    AircraftState fromStringToAircraftState(const std::string& stateStr) {
        if (stateStr == "Idle") return AircraftState::Idle;
        if (stateStr == "InFlight") return AircraftState::InFlight;
        if (stateStr == "Takeoff") return AircraftState::Takeoff;
        if (stateStr == "Landing") return AircraftState::Landing;
        return AircraftState::Completed;
    }

    std::string SystemStateToString(SystemState state) {
        return state == SystemState::ON ? "ON" : "OFF";
    }

    SystemState fromStringToSystemState(const std::string& stateStr) {
        return stateStr == "ON" ? SystemState::ON : SystemState::OFF;
    }

    std::string FlightTypeToString(FlightType type) {
        if (type == FlightType::Cargo) return "Cargo";
        if (type == FlightType::Commercial) return "Commercial";
        return "Private";
    }

    FlightType fromStringToFlightType(const std::string& typeStr) {
        if (typeStr == "Cargo") return FlightType::Cargo;
        if (typeStr == "Commercial") return FlightType::Commercial;
        return FlightType::Private;
    }

    std::string CommunicationStateToString(CommunicationState state) {
        if (state == CommunicationState::Closed) return "Closed";
        if (state == CommunicationState::EstablishedConnection) return "EstablishedConnection";
        if (state == CommunicationState::Sending) return "Sending";
        return "Handoff";
    }

    CommunicationState fromStringToCommunicationState(const std::string& stateStr) {
        if (stateStr == "Closed") return CommunicationState::Closed;
        if (stateStr == "EstablishedConnection") return CommunicationState::EstablishedConnection;
        if (stateStr == "Sending") return CommunicationState::Sending;
        return CommunicationState::Handoff;
    }

    std::string AcarsFlagToString(ACARSFlag flag) {
        if (flag == ACARSFlag::Handoff) return "Handoff";
        if (flag == ACARSFlag::Data) return "Data";
        if (flag == ACARSFlag::Message) return "Message";
        if (flag == ACARSFlag::Acknowledge) return "Acknowledge";
        return "Request";
    }

    ACARSFlag fromStringToAcarsFlag(const std::string& flagStr) {
        if (flagStr == "Handoff") return ACARSFlag::Handoff;
        if (flagStr == "Data") return ACARSFlag::Data;
        if (flagStr == "Message") return ACARSFlag::Message;
        if (flagStr == "Acknowledge") return ACARSFlag::Acknowledge;
        return ACARSFlag::Request;
    }

    std::string CommunicationTypeToString(CommunicationType type) {
        return type == CommunicationType::VHF ? "VHF" : "HF";
    }

    CommunicationType fromStringToCommunicationType(const std::string& typeStr) {
        return typeStr == "VHF" ? CommunicationType::VHF : CommunicationType::HF;
    }
}
