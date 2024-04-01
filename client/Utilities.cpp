#include "Utilities.h"

namespace aircraft {

    std::string aircraftStateToString(AircraftState state) {
        std::string result;
        if (state == AircraftState::Idle) {
            result = "Idle";
        }
        else if (state == AircraftState::InFlight) {
            result = "InFlight";
        }
        else if (state == AircraftState::Takeoff) {
            result = "Takeoff";
        }
        else if (state == AircraftState::Landing) {
            result = "Landing";
        }
        else {
            result = "Completed";
        }
        return result;
    }

    AircraftState fromStringToAircraftState(const std::string& stateStr) {
        AircraftState state;
        if (stateStr == "Idle") {
            state = AircraftState::Idle;
        }
        else if (stateStr == "InFlight") {
            state = AircraftState::InFlight;
        }
        else if (stateStr == "Takeoff") {
            state = AircraftState::Takeoff;
        }
        else if (stateStr == "Landing") {
            state = AircraftState::Landing;
        }
        else {
            state = AircraftState::Completed;
        }
        return state;
    }

    std::string SystemStateToString(SystemState state) {
        return state == SystemState::ON ? "ON" : "OFF";
    }

    SystemState fromStringToSystemState(const std::string& stateStr) {
        return stateStr == "ON" ? SystemState::ON : SystemState::OFF;
    }

    std::string FlightTypeToString(FlightType type) {
        std::string result;
        if (type == FlightType::Cargo) {
            result = "Cargo";
        }
        else if (type == FlightType::Commercial) {
            result = "Commercial";
        }
        else {
            result = "Private";
        }
        return result;
    }

    FlightType fromStringToFlightType(const std::string& typeStr) {
        FlightType type;
        if (typeStr == "Cargo") {
            type = FlightType::Cargo;
        }
        else if (typeStr == "Commercial") {
            type = FlightType::Commercial;
        }
        else {
            type = FlightType::Private;
        }
        return type;
    }

    std::string CommunicationStateToString(CommunicationState state) {
        std::string result;
        if (state == CommunicationState::Closed) {
            result = "Closed";
        }
        else if (state == CommunicationState::EstablishedConnection) {
            result = "EstablishedConnection";
        }
        else if (state == CommunicationState::Sending) {
            result = "Sending";
        }
        else {
            result = "Handoff";
        }
        return result;
    }

    CommunicationState fromStringToCommunicationState(const std::string& stateStr) {
        CommunicationState state;
        if (stateStr == "Closed") {
            state = CommunicationState::Closed;
        }
        else if (stateStr == "EstablishedConnection") {
            state = CommunicationState::EstablishedConnection;
        }
        else if (stateStr == "Sending") {
            state = CommunicationState::Sending;
        }
        else {
            state = CommunicationState::Handoff;
        }
        return state;
    }

    std::string AcarsFlagToString(ACARSFlag flag) {
        std::string result;
        if (flag == ACARSFlag::Handoff) {
            result = "Handoff";
        }
        else if (flag == ACARSFlag::Data) {
            result = "Data";
        }
        else if (flag == ACARSFlag::Message) {
            result = "Message";
        }
        else if (flag == ACARSFlag::Acknowledge) {
            result = "Acknowledge";
        }
        else {
            result = "Request";
        }
        return result;
    }

    ACARSFlag fromStringToAcarsFlag(const std::string& flagStr) {
        ACARSFlag flag;
        if (flagStr == "Handoff") {
            flag = ACARSFlag::Handoff;
        }
        else if (flagStr == "Data") {
            flag = ACARSFlag::Data;
        }
        else if (flagStr == "Message") {
            flag = ACARSFlag::Message;
        }
        else if (flagStr == "Acknowledge") {
            flag = ACARSFlag::Acknowledge;
        }
        else {
            flag = ACARSFlag::Request;
        }
        return flag;
    }

    std::string CommunicationTypeToString(CommunicationType type) {
        return type == CommunicationType::VHF ? "VHF" : "HF";
    }

    CommunicationType fromStringToCommunicationType(const std::string& typeStr) {
        return typeStr == "VHF" ? CommunicationType::VHF : CommunicationType::HF;
    }
}
