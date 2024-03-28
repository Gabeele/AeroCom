#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <thread>
#include <iostream>
#include <math.h>

#include "logger.h"
#include "communicationSystem.h"
#include "ACARS.h"


namespace aircraft {

    enum class AircraftState { Idle, InFlight, Takeoff, Landing, Completed };
    enum class SystemState { ON, OFF };
    enum class FlightType { Cargo, Commercial, Private };
    enum class CommunicationState { Closed, EstablishedConnection, Sending, Handoff };

    class Aircraft {
    public:
        explicit Aircraft(const std::string& identifier);
        Aircraft(const std::string& identifier, AircraftState state, SystemState commsState, SystemState acarsState,
            unsigned int flightNumber, FlightType flightType, const std::string& departureAirport, const std::string& arrivalAirport,
            float latitude, float longitude, float altitude, float speed, unsigned int heading);
        void updateAircraftState(AircraftState newState);
        void toggleCommunicationSystem();
        void toggleACARSSystem();
        void loadFlightPlan(const std::string& filepath);
        void toggleSimulateTelemetry();
        AircraftState getAircraftState();

    private:
        void acarsOperation();
        void simulateTelemetryOperation();

        std::thread acarsThread;
        std::atomic<bool> acarsActive{ false };

        std::thread simulateTelemetryThread;
        std::atomic<bool> simulateTelemetryActive{ false };

        CommunicationSystem comms;

        std::string identifier;
        AircraftState state;
        SystemState simulateState;
        SystemState commsToggle;
        SystemState acarsToggle;
        CommunicationState commsState;

        bool flightPlanLoaded ;
        bool communicationReady;


        struct flightInformation {

            unsigned int number;
            FlightType type;
            std::string departureAirport;
            std::string arrivalAirport;
        } flightInfo;

        struct telemetry {
            float latitude;
            float longitude;
            float altitude;
            float speed;
            unsigned int heading;
        } flightTelemetry;


    };
#endif
}
