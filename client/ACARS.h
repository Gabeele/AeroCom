#ifndef ACARS_H
#define ACARS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "logger.h"


namespace aircraft {

    enum class ACARSFlag { Handoff, Data, Message, Acknowledge, Request };

    class ACARS {
    public:
        ACARS();

        ACARS(unsigned int transmissionNumber, bool messagePriority, ACARSFlag flag,
            const std::string& aircraftID, const std::string& groundStationID,
            const std::string& flightNumber, const std::string& aircraftType,
            const std::string& departureAirport, const std::string& destinationAirport,
            float latitude, float longitude, float altitude,
            float speed, unsigned int heading);

        void setTransmissionNumber(unsigned int number);
        void setIsPriority(bool priority);
        void setFlag(ACARSFlag flag);
        void setAircraftID(const std::string& id);
        void setGroundStationID(const std::string& id);
        void setFlightInformation(const std::string& flightNumber, const std::string& aircraftType, const std::string& departureAirport, const std::string& destinationAirport);
        void setTelemetryData(float latitude, float longitude, float altitude, float speed, unsigned int heading);

        std::string generateChecksum(const std::string& packetContent) const;
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

        std::string getFormattedTimestamp();
    };

#endif
}
