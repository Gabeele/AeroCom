#include "aircraft.h"

namespace aircraft {

    Aircraft::Aircraft(const std::string& identifier) :
        identifier(identifier),
        state(AircraftState::InFlight),
        commsToggle(SystemState::OFF),
        acarsToggle(SystemState::OFF),
        flightInfo{ 0, FlightType::Commercial, "Unknown", "Unknown" },
        flightTelemetry{ 0.0, 0.0, 0.0, 0.0, 0 } {

    }
   
    void Aircraft::updateAircraftState(AircraftState newState) {
        // Convert the newState enum to a descriptive string
        std::string newStateStr;
        switch (newState) {
        case AircraftState::Idle:
            newStateStr = "Idle";
            break;
        case AircraftState::InFlight:
            newStateStr = "In Flight";
            break;
        case AircraftState::Takeoff:
            newStateStr = "Takeoff";
            break;
        case AircraftState::Landing:
            newStateStr = "Landing";
            break;
        case AircraftState::Completed:
            newStateStr = "Completed";
            break;
        default:
            newStateStr = "Unknown State";
            break;
        }


        logs::logger.log("Updated aircraft state to: " + newStateStr, logs::Logger::LogLevel::Info);


        state = newState;
    }

    void Aircraft::toggleCommunicationSystem() {
        if (commsToggle == SystemState::ON) {
            commsToggle = SystemState::OFF;
            if (!comms.disconnect()) {
                logs::logger.log("Failed to disconnect communication system.", logs::Logger::LogLevel::Error);

            }
            commsState = CommunicationState::Closed;

            listeningActive = false;
            if (listeningThread.joinable()) {
                listeningThread.join();
            }

        }
        else {
            this->communicationReady = false;
            commsToggle = SystemState::ON;
            if (!comms.connect()) {
                logs::logger.log("Failed to connect communication system.", logs::Logger::LogLevel::Error);
                commsState = CommunicationState::Closed;
                logs::logger.log("Communicate state: closed", logs::Logger::LogLevel::Info);


            }
            else {
                commsState = CommunicationState::EstablishedConnection;
                logs::logger.log("Communicate state: Established Connection", logs::Logger::LogLevel::Info);
                this->communicationReady = true;

                listeningActive = true;
                listeningThread = std::thread(&Aircraft::listeningOperation, this);

            }
        }

        std::string commsStateString = (commsToggle == SystemState::ON) ? "ON" : "OFF";
        logs::logger.log("Communication switch toggled: " + commsStateString, logs::Logger::LogLevel::Info);


    }

    void Aircraft::toggleACARSSystem() {

        acarsToggle = (acarsToggle == SystemState::ON) ? SystemState::OFF : SystemState::ON;
        if (acarsToggle == SystemState::ON && commsToggle == SystemState::ON) {
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

        logs::logger.log("Loading flight plan...", logs::Logger::LogLevel::Info);

        this->flightPlanLoaded = false;

        std::ifstream file(filepath);
        if (!file) {
            logs::logger.log("Error loading the flight plan. Is the file missing?", logs::Logger::LogLevel::Error);

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

            this->flightPlanLoaded = true;
            logs::logger.log("Flight plan is loaded.", logs::Logger::LogLevel::Info);

        }
    }

    void Aircraft::acarsOperation() {
        commsState = CommunicationState::Sending;
        logs::logger.log("Communicate state: sending", logs::Logger::LogLevel::Info);
        while (acarsActive) {

            if (!flightPlanLoaded || !communicationReady) {
                continue;
            }

            ACARS buf;

            // Increment transmission number for each new packet
            static unsigned int transmissionNumber = 0;
            buf.setTransmissionNumber(++transmissionNumber);

            buf.setIsPriority(true);
            buf.setFlag(ACARSFlag::Data);

            std::string aircraftTypeStr;
            switch (this->flightInfo.type) {
            case FlightType::Cargo:
                aircraftTypeStr = "Cargo";
                break;
            case FlightType::Commercial:
                aircraftTypeStr = "Commercial";
                break;
            case FlightType::Private:
                aircraftTypeStr = "Private";
                break;
            default:
                aircraftTypeStr = "Unknown";
                break;
            }

            buf.setAircraftID(this->identifier);
            buf.setFlightInformation(
                std::to_string(this->flightInfo.number),
                aircraftTypeStr,
                this->flightInfo.departureAirport,
                this->flightInfo.arrivalAirport
            );

            buf.setTelemetryData(
                this->flightTelemetry.latitude,
                this->flightTelemetry.longitude,
                this->flightTelemetry.altitude,
                this->flightTelemetry.speed,
                this->flightTelemetry.heading
            );

            std::string packet = buf.serializePacket();

            bool result = comms.sendMessage(packet);

            if (result) {
                logs::logger.log("Packet sent.", logs::Logger::LogLevel::Info);
            }
            else {
                logs::logger.log("Failed to send packet.", logs::Logger::LogLevel::Error);
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    AircraftState Aircraft::getAircraftState() {
        return this->state;
    }

    void Aircraft::toggleSimulateTelemetry() {
        simulateState = (simulateState == SystemState::ON) ? SystemState::OFF : SystemState::ON;
        if (simulateState == SystemState::ON) {
            simulateTelemetryActive = true;
            simulateTelemetryThread = std::thread(&Aircraft::simulateTelemetryOperation, this);
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

        updateAircraftState(AircraftState::Idle);


        while (simulateTelemetryActive) {
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = currentTime - startTime;
            float elapsedTime = elapsed.count();

            const float ascentDuration = 60.0; // Ascent duration in seconds
            const float cruisingStartTime = 60.0; // When cruising starts in seconds
            const float descentStartTime = 90.0; // When descent starts in seconds
            const float totalDuration = 120.0; // Total flight duration in seconds

            // Calculate the phase of the flight
            if (elapsedTime <= ascentDuration) {
                updateAircraftState(AircraftState::Takeoff);
                flightTelemetry.altitude = maxAltitude * (elapsedTime / ascentDuration);
                flightTelemetry.speed = maxSpeed * (elapsedTime / ascentDuration);
            }
            else if (elapsedTime > cruisingStartTime && elapsedTime <= descentStartTime) {
                flightTelemetry.altitude = maxAltitude;
                flightTelemetry.speed = maxSpeed;
                updateAircraftState(AircraftState::InFlight);
            }
            else if (elapsedTime > descentStartTime && elapsedTime <= totalDuration) {
                flightTelemetry.altitude = maxAltitude * (1.0 - ((elapsedTime - descentStartTime) / (totalDuration - descentStartTime)));
                flightTelemetry.speed = maxSpeed * (1.0 - ((elapsedTime - descentStartTime) / (totalDuration - descentStartTime)));
                updateAircraftState(AircraftState::Landing);
            }
            else {
                flightTelemetry.altitude = 0.0;
                flightTelemetry.speed = 0.0;
                updateAircraftState(AircraftState::Completed);
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

    void Aircraft::setFrequencyChannel(const std::string frequency, const std::string channel) {

        comms.setFrequency(frequency);
        comms.setChannel(channel);

    }

    void Aircraft::listeningOperation() {
        std::string handoffFlag = "Flag: H";

        while (listeningActive) {
            std::string message = comms.receiveMessage();
            if (message.find(handoffFlag) != std::string::npos) {
                std::string newFrequency = extractFrequency(message);
                std::string newChannel = extractChannel(message);
                if (comms.handoff(newFrequency, newChannel)) {
                    logs::logger.log("Handoff completed. Connected to new server.", logs::Logger::LogLevel::Info);
                }
                else{
                    logs::logger.log("Handoff didn't work.", logs::Logger::LogLevel::Error);
            }
            }
        }
    }

    std::string Aircraft::extractFrequency(const std::string& message) {
        std::string result = DEFAULT_FREQUENCY; 
        std::string frequencyLabel = "Frequency: ";

        auto startPos = message.find(frequencyLabel);
        if (startPos != std::string::npos) {
        std::string space = " ";    
            startPos += frequencyLabel.length();
            auto endPos = message.find(space, startPos);
            if (endPos == std::string::npos) {
                endPos = message.length();
            }
            result = message.substr(startPos, endPos - startPos); 
        }
        return result; // Single return statement
    }

    std::string Aircraft::extractChannel(const std::string& message) {
        std::string result = DEFAULT_CHANNEL_STR; 
        std::string channelLabel = "Channel: ";

        auto startPos = message.find(channelLabel);
        if (startPos != std::string::npos) {
        std::string space = " ";
            startPos += channelLabel.length();
            auto endPos = message.find(space, startPos);
            if (endPos == std::string::npos) {
                endPos = message.length();
            }
            result = message.substr(startPos, endPos - startPos); 
        }
        return result; 
    }

}

