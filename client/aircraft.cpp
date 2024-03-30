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

        state = newState;

        logs::logger.log("Updated aircraft state to: " + aircraft::aircraftStateToString(state), logs::Logger::LogLevel::Info);
    }

    AircraftState Aircraft::getAircraftState() {
        return this->state;
    }

    void aircraft::Aircraft::setCommState(CommunicationState newState) 
    {
            this->commsState = newState;
            logs::logger.log("Communication state set to: " + CommunicationStateToString(newState), logs::Logger::LogLevel::Info);

     }
    void Aircraft::toggleCommunicationSystem() {

        if (commsToggle == SystemState::ON) {

            commsToggle = SystemState::OFF;     // Toggle off

            this->listeningActive = false;    // Set listen to false

            if (listeningThread.joinable()) {   // Join the threads
                listeningThread.join();
            }
            if (comms.disconnect()) {      // Attempt to disconnect

                setCommState(CommunicationState::Closed);   // Close the comm state

            }

        }
        else {

            commsToggle = SystemState::ON;  // Toggle On

            this->communicationReady = false;

            if (!comms.connect()) {

                logs::logger.log("Failed to connect communication system.", logs::Logger::LogLevel::Error);
                setCommState(CommunicationState::Closed);

            }
            else {
                setCommState(CommunicationState::EstablishedConnection);

                this->communicationReady = true;
                this->listeningActive = true;

                listeningThread = std::thread(&Aircraft::listeningOperation, this);

            }
        }

        logs::logger.log("Communication toggled: " + SystemStateToString(commsToggle), logs::Logger::LogLevel::Info);

    }

    void Aircraft::toggleACARSSystem() {

        acarsToggle = (acarsToggle == SystemState::ON) ? SystemState::OFF : SystemState::ON;    // Toggle

        if (acarsToggle == SystemState::ON && commsToggle == SystemState::ON) {     // Activate the acars thread 
            acarsActive = true;

            acarsThread = std::thread(&Aircraft::acarsOperation, this);

        }
        else {  // Otherwise it should be joined

            acarsActive = false;

            if (acarsThread.joinable()) {
                acarsThread.join();
            }
        }
    }

    void Aircraft::toggleSimulateTelemetry() {

        simulateState = (simulateState == SystemState::ON) ? SystemState::OFF : SystemState::ON;    // Toggle

        if (simulateState == SystemState::ON) {     // Start the simulation thread

            simulateTelemetryActive = true;

            simulateTelemetryThread = std::thread(&Aircraft::simulateTelemetryOperation, this);
        }
        else {  // Otherwise join

            simulateTelemetryActive = false;

            if (simulateTelemetryThread.joinable()) {
                simulateTelemetryThread.join();
            }
        }
    }

    bool Aircraft::loadFlightPlan(const std::string & filepath) {
        bool success = true;

        logs::logger.log("Loading flight plan...", logs::Logger::LogLevel::Info);

        this->flightPlanLoaded = false;

        std::ifstream file(filepath);

        if (!file) {
            logs::logger.log("Error loading the flight plan. Is the file missing?", logs::Logger::LogLevel::Error);
            success = false;
        }

        if (success) {
            std::string line;

            // Read Flight Number
            if (std::getline(file, line)) {
                flightInfo.number = std::stoul(line);
                success = true;
            }
            else {
                success = false;
            }

            // Read Flight Type
            if (success && std::getline(file, line)) {
                flightInfo.type = fromStringToFlightType(line);
                success = true;

            }
            else {
                success = false;
            }

            // Read Departure Airport
            if (success && std::getline(file, line)) {
                flightInfo.departureAirport = line;
                success = true;

            }
            else {
                success = false;
            }

            // Read Arrival Airport
            if (success && std::getline(file, line)) {
                flightInfo.arrivalAirport = line;
                success = true;

            }
            else {
                success = false;
            }
        }

        file.close();

        if (success) {
            this->flightPlanLoaded = true;
            logs::logger.log("Flight plan is loaded.", logs::Logger::LogLevel::Info);
        }
        else {
            logs::logger.log("Failed to load the flight plan.", logs::Logger::LogLevel::Error);
        }

        return success;

    }

    void Aircraft::acarsOperation() {

        setCommState(CommunicationState::Sending);

        while (acarsActive) {

            if (!flightPlanLoaded || !communicationReady) {     // Block any packets from being built if the flight plan isn't loaded or communicaiton isn't ready
                continue;
            }


            // Building acars packet using the current flight data. 

            ACARS buf;

            static unsigned int transmissionNumber = 0;
            buf.setTransmissionNumber(++transmissionNumber);

            buf.setIsPriority(true);
            buf.setFlag(ACARSFlag::Data);

            std::string aircraftTypeStr = FlightTypeToString(this->flightInfo.type);

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

            if (!result) {
                logs::logger.log("Error sending ACARS packet from aircraft.", logs::Logger::LogLevel::Error);
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
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

    void Aircraft::setFrequencyChannel(const std::string & frequency, const std::string & channel) {

        comms.setFrequency(frequency);
        comms.setChannel(channel);

    }

    void Aircraft::listeningOperation() {
        std::string handoffFlag = "Flag: H";    // Flag patter for handoff request

        while (listeningActive) {

            std::string message = comms.receiveMessage();   // Listen for receive 

            // Find and extract the frequency and channel
            if (message.find(handoffFlag) != std::string::npos) {

                std::string newFrequency = extractFrequency(message);
                std::string newChannel = extractChannel(message);

                if (comms.handoff(newFrequency, newChannel)) {
                    logs::logger.log("Handoff completed. Connected to new server.", logs::Logger::LogLevel::Info);
                }
                else {
                    logs::logger.log("Handoff didn't work.", logs::Logger::LogLevel::Error);
                }
            }
        }
    }

    std::string Aircraft::extractFrequency(const std::string & message) {

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
        return result; 
    }

    std::string Aircraft::extractChannel(const std::string & message) {

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

