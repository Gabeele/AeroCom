// This file is used to denote the aircraft functions and properties
#pragma once

#include <string>
#include <thread>
#include <iostream>
#include <math.h>

#include "logger.h"
#include "communicationSystem.h"
#include "ACARS.h"
#include "Utilities.h"


namespace aircraft {

    class Aircraft {
    public:
        /// <summary>
        /// Constructor of aircraft.
        /// </summary>
        /// <param name="identifier">A string identifer</param>
        Aircraft(const std::string& identifier);


        /// <summary>
        /// 
        /// </summary>
        /// <param name="newState"></param>
        void updateAircraftState(AircraftState newState);

        /// <summary>
        /// 
        /// </summary>
        void toggleCommunicationSystem();

        /// <summary>
        /// 
        /// </summary>
        void toggleACARSSystem();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="filepath"></param>
        void loadFlightPlan(const std::string& filepath);

        /// <summary>
        /// 
        /// </summary>
        void toggleSimulateTelemetry();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        AircraftState getAircraftState();

        /// <summary>
        /// 
        /// </summary>
        void setFrequencyChannel(const std::string frequency, const std::string channel);

    private:
        /// <summary>
        /// 
        /// </summary>
        void acarsOperation();

        /// <summary>
        /// 
        /// </summary>
        void simulateTelemetryOperation();

        /// <summary>
        /// 
        /// </summary>
        void listeningOperation();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>
        std::string extractFrequency(const std::string& message);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <returns></returns>
        std::string extractChannel(const std::string& message);

        std::thread acarsThread;                                    // Threading variables and atomic booleans
        std::atomic<bool> acarsActive{ false };

        std::thread simulateTelemetryThread;
        std::atomic<bool> simulateTelemetryActive{ false };

        std::thread listeningThread;
        std::atomic<bool> listeningActive{ false };

        CommunicationSystem comms;          // The communication system

        AircraftState state;                // Current state of the aircraft
        SystemState simulateState;          // System state of the simulation (ON/OFF)
        SystemState commsToggle;            // System state of the communcations 
        SystemState acarsToggle;            // System state of ACARS
        CommunicationState commsState;      // Current state of communications  (Connected/Disconnected/etc)
            
        bool flightPlanLoaded;          // Boolean for if the flight plan is loaded
        bool communicationReady;        // Boolean for when communications are ready

        // Plane informaiton and details
        std::string identifier;     

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
}
