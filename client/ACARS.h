// This file denotes the ACARS functions and properties
#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "Utilities.h"
#include "logger.h"



namespace aircraft {

    class ACARS {
    public:
        ACARS();

        /// <summary>
        /// Parameterized constructor for ACARS.
        /// Sets up an ACARS instance with detailed message and flight information.
        /// </summary>
        /// <param name="transmissionNumber">Unique number identifying the transmission.</param>
        /// <param name="messagePriority">Boolean indicating if the message is of high priority.</param>
        /// <param name="flag">Type of message being sent, identified by an ACARSFlag enum.</param>
        /// <param name="aircraftID">Unique identifier for the aircraft.</param>
        /// <param name="groundStationID">Identifier for the ground station intended to receive the message.</param>
        /// <param name="flightNumber">Flight number of the aircraft.</param>
        /// <param name="aircraftType">Type of aircraft.</param>
        /// <param name="departureAirport">Identifier of the departure airport.</param>
        /// <param name="destinationAirport">Identifier of the destination airport.</param>
        /// <param name="latitude">Current latitude of the aircraft.</param>
        /// <param name="longitude">Current longitude of the aircraft.</param>
        /// <param name="altitude">Current altitude of the aircraft in feet.</param>
        /// <param name="speed">Current speed of the aircraft in knots.</param>
        /// <param name="heading">Current heading of the aircraft in degrees.</param>
        ACARS(unsigned int transmissionNumber, bool messagePriority, ACARSFlag flag,
            const std::string& aircraftID, const std::string& groundStationID,
            const std::string& flightNumber, const std::string& aircraftType,
            const std::string& departureAirport, const std::string& destinationAirport,
            float latitude, float longitude, float altitude,
            float speed, unsigned int heading);

        /// <summary>
        /// Sets the transmission number for the ACARS message.
        /// </summary>
        /// <param name="number">The transmission number.</param>
        void setTransmissionNumber(unsigned int number);

        /// <summary>
        /// Sets the message priority status.
        /// </summary>
        /// <param name="priority">True if the message is of high priority.</param>
        void setIsPriority(bool priority);

        /// <summary>
        /// Sets the type of ACARS message.
        /// </summary>
        /// <param name="flag">The message type, represented by an ACARSFlag enum.</param>
        void setFlag(ACARSFlag flag);

        /// <summary>
        /// Sets the aircraft's unique identifier.
        /// </summary>
        /// <param name="id">The aircraft ID.</param>
        void setAircraftID(const std::string& id);

        /// <summary>
        /// Sets the ground station's unique identifier.
        /// </summary>
        /// <param name="id">The ground station ID.</param>
        void setGroundStationID(const std::string& id);

        /// <summary>
        /// Sets the flight information.
        /// </summary>
        /// <param name="flightNumber">The flight number.</param>
        /// <param name="aircraftType">The aircraft type.</param>
        /// <param name="departureAirport">The departure airport.</param>
        /// <param name="destinationAirport">The destination airport.</param>
        void setFlightInformation(const std::string& flightNumber, const std::string& aircraftType,
            const std::string& departureAirport, const std::string& destinationAirport);

        /// <summary>
        /// Sets the telemetry data for the message.
        /// </summary>
        /// <param name="latitude">Latitude of the aircraft.</param>
        /// <param name="longitude">Longitude of the aircraft.</param>
        /// <param name="altitude">Altitude of the aircraft.</param>
        /// <param name="speed">Speed of the aircraft.</param>
        /// <param name="heading">Heading of the aircraft.</param>
        void setTelemetryData(float latitude, float longitude, float altitude,
            float speed, unsigned int heading);

        /// <summary>
        /// Generates a checksum for the given packet content.
        /// </summary>
        /// <param name="packetContent">The content of the packet.</param>
        /// <returns>A string representing the checksum.</returns>
        std::string generateChecksum(const std::string& packetContent) const;

        /// <summary>
        /// Serializes the ACARS message into a string format suitable for transmission.
        /// </summary>
        /// <returns>A serialized string representation of the ACARS message.</returns>
        std::string serializePacket();

    private:
        unsigned int transmissionNumber;
        bool isPriority;
        ACARSFlag flag;
        std::string aircraftID;
        std::string groundStationID;
        std::string message;
        float latitude;
        float longitude;
        float altitude;
        float speed;
        unsigned int heading;
        std::string timestamp;

        // Flight Information
        std::string flightNumber;
        std::string aircraftType;
        std::string departureAirport;
        std::string destinationAirport;

        /// <summary>
        /// Gets the current timestamp
        /// </summary>
        /// <returns>A string formatted timestamp</returns>
        std::string getFormattedTimestamp();
    };


}
