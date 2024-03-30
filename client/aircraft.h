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
        /// <param name="identifier">A unique string identifier for the aircraft, typically an alphanumeric code like a tail number or call sign.</param>
        Aircraft(const std::string& identifier);

        /// <summary>
        /// Updates the state of the aircraft to a new state.
        /// </summary>
        /// <param name="newState">The new state to which the aircraft should be updated. It is of type AircraftState which can be Idle, InFlight, Takeoff, Landing, or Completed.</param>
        void updateAircraftState(AircraftState newState);

        /// <summary>
        /// Toggles the communication system of the aircraft on or off.
        /// When invoked, it changes the state of the communication system to its opposite state (ON to OFF or OFF to ON).
        /// </summary>
        void toggleCommunicationSystem();

        /// <summary>
        /// Toggles the ACARS (Aircraft Communication Addressing and Reporting System) system of the aircraft on or off.
        /// When invoked, it changes the state of the ACARS system to its opposite state (ON to OFF or OFF to ON).
        /// </summary>
        void toggleACARSSystem();

        /// <summary>
        /// Loads a flight plan from a specified file.
        /// </summary>
        /// <param name="filepath">The path to the file containing the flight plan data.</param>
        bool loadFlightPlan(const std::string& filepath);

        /// <summary>
        /// Toggles the simulation of telemetry data for the aircraft on or off.
        /// When invoked, it changes the state of the telemetry simulation to its opposite state (ON to OFF or OFF to ON).
        /// </summary>
        void toggleSimulateTelemetry();

        /// <summary>
        /// Retrieves the current state of the aircraft.
        /// </summary>
        /// <returns>A value from the AircraftState enum representing the current state of the aircraft.</returns>
        AircraftState getAircraftState();

        /// <summary>
        /// Sets the frequency and channel for the aircraft's communication system.
        /// </summary>
        /// <param name="frequency">The frequency to which the communication system should be set.</param>
        /// <param name="channel">The channel to which the communication system should be set.</param>
        void setFrequencyChannel(const std::string& frequency, const std::string& channel);

    private:
        /// <summary>
        /// Sets the communicate state
        /// </summary>
        /// <param name="newState">A new communication state</param>
        void setCommState(CommunicationState newState);

        /// <summary>
        /// Handles the operation of the ACARS system.
        /// This function is meant to be run in a separate thread to continuously handle ACARS operations.
        /// </summary>
        void acarsOperation();

        /// <summary>
        /// Simulates telemetry data for the aircraft.
        /// This function is meant to be run in a separate thread to simulate telemetry data generation.
        /// </summary>
        void simulateTelemetryOperation();

        /// <summary>
        /// Listens for incoming communication and handles it accordingly.
        /// This function is meant to be run in a separate thread to continuously listen for and process incoming messages.
        /// </summary>
        void listeningOperation();

        /// <summary>
        /// Extracts the frequency from a given message.
        /// </summary>
        /// <param name="message">A string message from which the frequency should be extracted.</param>
        /// <returns>The extracted frequency as a string.</returns>
        std::string extractFrequency(const std::string& message);

        /// <summary>
        /// Extracts the communication channel from a given message.
        /// </summary>
        /// <param name="message">A string message from which the channel should be extracted.</param>
        /// <returns>The extracted channel as a string.</returns>
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
