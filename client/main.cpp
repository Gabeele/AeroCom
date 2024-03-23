#include <iostream>
#include "aircraft.h"

int main() {

    // Just testing the communication system. 
    aircraft::Aircraft ac("Hello");
    ac.loadFlightPlan("FlightPlan.txt");
    ac.toggleCommunicationSystem();
    ac.toggleSimulateTelemetry();
    ac.toggleACARSSystem();


    return 0;
}
