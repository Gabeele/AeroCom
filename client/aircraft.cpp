#include "Aircraft.h"
#include <iostream>



namespace aircraft {

    // Constructor
    Aircraft::Aircraft(const std::string& identifier) :
        identifier(identifier),
        state(AircraftState::InFlight),
        commsState(SystemState::OFF),
        acarsState(SystemState::OFF),
        flightInfo{ 0, FlightType::Commercial, "Unknown", "Unknown" },
        flightTelemetry{ 0.0, 0.0, 0.0, 0.0, 0 } { 
     
    }

    Aircraft::Aircraft(const std::string& identifier, AircraftState state, SystemState commsState, SystemState acarsState,
        unsigned int flightNumber,FlightType flightType, const std::string& departureAirport, const std::string& arrivalAirport,
        float latitude, float longitude, float altitude, float speed, unsigned int heading)
        : identifier(identifier), state(state), commsState(commsState), acarsState(acarsState),
        flightInfo{ flightNumber, flightType, departureAirport, arrivalAirport },
        flightTelemetry{ latitude, longitude, altitude, speed, heading } {
    }

    void Aircraft::updateAircraftState(AircraftState newState) {
        state = newState;

    }

    void Aircraft::toggleCommunicationSystem() {
        commsState = (commsState == SystemState::ON) ? SystemState::OFF : SystemState::ON;

        // TODO comms should be a thread to listen for incoming packets and then parse out what to do with them
    }

    void Aircraft::toggleACARSSystem() {
        acarsState = (acarsState == SystemState::ON) ? SystemState::OFF : SystemState::ON;
        if (acarsState == SystemState::ON && commsState == SystemState::ON) {
            acarsActive = true;
            acarsThread = std::thread(&Aircraft::acarsOperation, this);
        }
        else {
            acarsActive = false;
            if (acarsThread.joinable()) {
                acarsThread.join();
            }
        }
    }

    void Aircraft::acarsOperation() {
        while (acarsActive) {

            std::string packet = acars.serializePacket(); 
            comms.sendMessage(packet);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

   void Aircraft::dashboard ()  {
        std::cout << "Dashboard Display:\n";
        std::cout << "Aircraft Identifier: " << identifier << "\n";
        std::cout << "Aircraft State: " << static_cast<int>(state) << "\n"; 
        
    }


    void Aircraft::input() {
        std::cout << "Enter command (1: Toggle Comms, 2: Toggle ACARS, 3: Change State): ";

        int command = 0;
        std::cin >> command;

        while (std::cin.get() != '\n') {

        }

        if (!std::cin.fail()) {
            switch (command) {
            case 1:
                toggleCommunicationSystem();
                break;
            case 2:
                toggleACARSSystem();
                break;
            case 3:
                break;
            default:
                std::cout << "Invalid command.\n";
                break;
            }
        }
        else {
            std::cin.clear();
            std::cout << "Invalid input, please enter a number.\n";
        }
    }

    
    void Aircraft::simulateFlightData() {
        
    }

    
    void Aircraft::simulateTelemetry() {
        
    }

} 
