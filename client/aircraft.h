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

    enum class AircraftState { Idle, InFlight, Takeoff, Landing };
    enum class SystemState { ON, OFF };
    enum class FlightType { Cargo, Commercial, Private };


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

        void dashboard();
        void input();

    private:
        void acarsOperation();
        void simulateTelemetryOperation();

        std::thread acarsThread;
        std::atomic<bool> acarsActive{ false };

        std::thread simulateTelemetryThread;
        std::atomic<bool> simulateTelemetryActive{ false };

        CommunicationSystem comms;
        ACARS acars;

        std::string identifier;
        AircraftState state;
        SystemState simulateState;
        SystemState commsState;
        SystemState acarsState;

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
