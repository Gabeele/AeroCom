#include "aircraft.h"

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
        unsigned int flightNumber, FlightType flightType, const std::string& departureAirport, const std::string& arrivalAirport,
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

        std::string commsStateString = (commsState == SystemState::ON) ? "ON" : "OFF";
        logs::logger.log("Communication switch toggled: " + commsStateString, logs::Logger::LogLevel::Info);

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

    void Aircraft::loadFlightPlan(const std::string& filepath) {
        bool exit = false;
        std::ifstream file(filepath);
        if (!file) {
            exit = true;
        }

        if (exit == false) {
            std::string line;

            // Read Flight Number
            if (std::getline(file, line)) {
                flightInfo.number = std::stoul(line);
            }

            // Read Flight Type
            if (std::getline(file, line)) {

                if (line == "Cargo") {
                    flightInfo.type = FlightType::Cargo;
                }
                else if (line == "Private") {
                    flightInfo.type = FlightType::Private;
                }
                else {
                    flightInfo.type = FlightType::Commercial;
                }
            }

            // Read Departure Airport
            if (std::getline(file, line)) {
                flightInfo.departureAirport = line;
            }

            // Read Arrival Airport
            if (std::getline(file, line)) {
                flightInfo.arrivalAirport = line;
            }

            file.close();
        }
    }

    void Aircraft::acarsOperation() {
        while (acarsActive) {

            acars.setTelemetryData(
                flightTelemetry.latitude,
                flightTelemetry.longitude,
                flightTelemetry.altitude,
                flightTelemetry.speed,
                flightTelemetry.heading
            );

            std::stringstream ss;

            std::string packet = acars.serializePacket();

            bool result = comms.sendMessage(packet);

            if (result) {
                std::cout << "Packet sent successfully!" << std::endl;
            }
            else {
                std::cerr << "Failed to send packet." << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }


    void Aircraft::dashboard() {
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


    void Aircraft::toggleSimulateTelemetry() {
        simulateState = (simulateState == SystemState::ON) ? SystemState::OFF : SystemState::ON;
        if (simulateState == SystemState::ON) {
            simulateTelemetryActive = true;
            simulateTelemetryThread = std::thread(&Aircraft::acarsOperation, this);
        }
        else {
            simulateTelemetryActive = false;
            if (simulateTelemetryThread.joinable()) {
                simulateTelemetryThread.join();
            }
        }
    }
    void Aircraft::simulateTelemetryOperation() {

        const double PI = 3.14159265358979323846;

        // Coordinates for JFK and LAX
        const float startLat = 40.641766; // JFK Latitude
        const float startLon = -73.780968; // JFK Longitude
        const float endLat = 33.941589; // LAX Latitude
        const float endLon = -118.40853; // LAX Longitude

        // Simulated flight parameters
        const float maxAltitude = 35000.0; // Maximum cruising altitude in feet
        const float maxSpeed = 500.0; // Cruising speed in knots

        auto startTime = std::chrono::steady_clock::now();

        while (simulateTelemetryActive) {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = currentTime - startTime;
            float elapsedTime = elapsed.count();

            // Convert integers to float explicitly for MISRA compliance
            const float ascentDuration = 900.0; // Ascent duration in seconds
            const float cruisingStartTime = 900.0; // When cruising starts in seconds
            const float descentStartTime = 17100.0; // When descent starts in seconds
            const float totalDuration = 18000.0; // Total flight duration in seconds

            // Calculate the phase of the flight
            if (elapsedTime <= ascentDuration) { // Ascent for first 15 minutes
                flightTelemetry.altitude = maxAltitude * (elapsedTime / ascentDuration);
                flightTelemetry.speed = maxSpeed * (elapsedTime / ascentDuration);
            }
            else if (elapsedTime > cruisingStartTime && elapsedTime <= descentStartTime) { // Cruising for next 4.5 hours
                flightTelemetry.altitude = maxAltitude;
                flightTelemetry.speed = maxSpeed;
            }
            else if (elapsedTime > descentStartTime && elapsedTime <= totalDuration) { // Descent for last 15 minutes
                flightTelemetry.altitude = maxAltitude * (1.0 - ((elapsedTime - descentStartTime) / (totalDuration - descentStartTime)));
                flightTelemetry.speed = maxSpeed * (1.0 - ((elapsedTime - descentStartTime) / (totalDuration - descentStartTime)));
            }
            else {
                flightTelemetry.altitude = 0.0;
                flightTelemetry.speed = 0.0;
            }

            // Linear interpolation for latitude and longitude over the flight duration
            float journeyFraction = elapsedTime / totalDuration; // Total flight duration
            flightTelemetry.latitude = startLat + (endLat - startLat) * journeyFraction;
            flightTelemetry.longitude = startLon + (endLon - startLon) * journeyFraction;

            // Simplified calculation for heading (not accounting for the curvature of the Earth)
            if (fabs(endLon - startLon) > 0.0) {
                flightTelemetry.heading = static_cast<unsigned int>(std::atan2(static_cast<double>(endLat - startLat), static_cast<double>(endLon - startLon)) * (180.0 / PI));
            }
            else {
                flightTelemetry.heading = (endLat > startLat) ? 0 : 180;
            }

            // Update at a fixed time interval
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

}