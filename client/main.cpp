#include <iostream>
#include "aircraft.h"

int main() {
    // Initialize the aircraft with an identifier
    aircraft::Aircraft ac("N12345");

    // Load the flight plan from a predefined file
    ac.loadFlightPlan("./FlightPlan.txt");

    // Toggle the communication system to active
    ac.toggleCommunicationSystem();

    // Toggle the ACARS system to active
    ac.toggleACARSSystem();

    // Start simulating telemetry data
    ac.toggleSimulateTelemetry();


    std::cout << "Simulating for 60 seconds. Please wait..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(60));

    std::cout << "Ending simulation." << std::endl;

    ac.toggleACARSSystem();

    ac.toggleCommunicationSystem();

    return 0;
}
