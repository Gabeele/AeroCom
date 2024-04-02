/// CSCN74000 - Software Safety and Reliability 
/// Group Project 
/// Group 1  - Lisa, Gavin, Thomas 
/// 
/// AeroCom - Simulation of auotmated handoff between two ATC systems

#include <iostream>
#include "aircraft.h"
#include "logger.h"

int main() {
    // Initialize the aircraft with an identifier
    aircraft::Aircraft ac("N12345");

    // Load the flight plan from a predefined file
    if (!ac.loadFlightPlan("./FlightPlan.txt")) {
        return 0;
    }

    //ac.setFrequencyChannel("192.168.1.33", "HF");

    // Toggle the communication system to active
    ac.toggleCommunicationSystem();

    // Toggle the ACARS system to active
    ac.toggleACARSSystem();

    // Start simulating telemetry data
    ac.toggleSimulateTelemetry();


    //// Keeps the system alive until the aircraft has completed it's trip via the simulation
    logs::logger.log("Starting simulation...", logs::Logger::LogLevel::Info);
    while (ac.getAircraftState() != aircraft::AircraftState::Completed) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    logs::logger.log("Simulation ended. Aircraft completed it's flight.", logs::Logger::LogLevel::Info);

    // Toggel simulation to deactive
    ac.toggleSimulateTelemetry();

    // Toggle ACARS system to deactive
    ac.toggleACARSSystem();

    // Toggle communication system to deactive
    ac.toggleCommunicationSystem();

    return 0;
}
