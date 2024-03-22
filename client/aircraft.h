#pragma once

#include <string>
#include "communications.h"

namespace aircraft {
    class Aircraft {
    public:
        /*enum class AircraftState { InFlight, Takeoff, Landing };*/

        Aircraft(const std::string& identifier);
        void initiateCommunication(CommunicationSystem type);
        void ACARS();
        //void updateAircraftState(AircraftState newState);

    private:
        //std::string identifier;
        //AircraftState state;
        CommunicationSystem comms; 

        //struct flightInformation {
        //    unsigned int number; 
        //    enum class type { Cargo, Commercial, Private};
        //    std::string departureAirport;
        //    std::string arrivalAirport;
        //};

        //struct telemetry {
        //    float latitude; 
        //    float longitude;
        //    float alititude;
        //    float speed; 
        //    unsigned int heading;
        //};

        // Helper functions
        void generateACARS();

    };
}
