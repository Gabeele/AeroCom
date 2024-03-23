#pragma once

#include <string>
#include <thread>
#include "CommunicationSystem.h"
#include "ACARS.h"

namespace aircraft {

    enum class AircraftState { Idle, InFlight, Takeoff, Landing };
    enum class SystemState { ON, OFF };
    enum class FlightType { Cargo, Commercial, Private };



    class Aircraft {
    public:
        Aircraft(const std::string& identifier);
        Aircraft(const std::string& identifier, AircraftState state, SystemState commsState, SystemState acarsState,
            unsigned int flightNumber, FlightType flightType, const std::string& departureAirport, const std::string& arrivalAirport,
            float latitude, float longitude, float altitude, float speed, unsigned int heading);
        void updateAircraftState(AircraftState newState);
        void toggleCommunicationSystem(); 
        void toggleACARSSystem(); 

        void dashboard();
        void input();



    private:

        void acarsOperation();

        std::thread acarsThread;
        std::atomic<bool> acarsActive{ false };

        std::string identifier;
        AircraftState state;
        CommunicationSystem comms;
        ACARS acars;
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



        // Helper functions
        void simulateFlightData();
        void simulateTelemetry();
    };

}
