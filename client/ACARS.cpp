#include "ACARS.h"

namespace aircraft {

    ACARS::ACARS() : transmissionNumber(0), isPriority(false), flag(ACARSFlag::Data),
        latitude(0.0), longitude(0.0), altitude(0.0),
        speed(0.0), heading(0) {}

    ACARS::ACARS(unsigned int transNum, bool priority, ACARSFlag flg,
        const std::string& acID, const std::string& gsID,
        const std::string& flNumber, const std::string& acType,
        const std::string& depAirport, const std::string& destAirport,
        float lat, float lon, float alt,
        float spd, unsigned int hdg)
        : transmissionNumber(transNum), isPriority(priority), flag(flg),
        aircraftID(acID), groundStationID(gsID),
        flightNumber(flNumber), aircraftType(acType),
        departureAirport(depAirport), destinationAirport(destAirport),
        latitude(lat), longitude(lon), altitude(alt),
        speed(spd), heading(hdg) {}

    std::string ACARS::generateChecksum(const std::string& packetContent) const {
        unsigned int checksum = 0;

        for (char c : packetContent) {
            checksum += static_cast<unsigned int>(c);
        }

        std::stringstream ss;
        ss << std::hex << checksum;

        return ss.str();
    }

    std::string ACARS::serializePacket() {
        std::stringstream packet;

        // Start of Packet
        packet << "START\n";


        packet << "Size: -\n"; // The size seems to always be the same so we won't need to add it

        packet << "Transmission Number: " << transmissionNumber << "\n";
        packet << "Message Priority: " << (isPriority ? "Yes" : "No") << "\n";

        char flagChar = ' ';

        switch (flag) {
        case ACARSFlag::Handoff: flagChar = 'H'; break;
        case ACARSFlag::Data:    flagChar = 'D'; break;
        case ACARSFlag::Acknowledge: flagChar = 'A'; break;
        case ACARSFlag::Message: flagChar = 'M'; break;
        case ACARSFlag::Request: flagChar = 'R'; break;
        default:                  flagChar = 'D'; break;
        }
        packet << "Flag: " << flagChar << "\n";

        packet << "Aircraft ID: " << aircraftID << "\n";
        packet << "Ground Station ID: " << groundStationID << "\n";

        // Flight Information
        packet << "Flight Information:\n";
        packet << "\tFlight Number: " << flightNumber << "\n";
        packet << "\tAircraft Type: " << aircraftType << "\n";
        packet << "\tDeparture Airport: " << departureAirport << "\n";
        packet << "\tDestination Airport: " << destinationAirport << "\n";

        // Location Details
        packet << "Location Details:\n";
        packet << "\tLatitude: " << latitude << "\n";
        packet << "\tLongitude: " << longitude << "\n";
        packet << "\tAltitude: " << altitude << "\n";
        packet << "\tSpeed: " << speed << "\n";
        packet << "\tHeading: " << heading << "\n";

        // Time Stamp
        packet << "Time Stamp: " << timestamp << "\n";

        // Message Terminator
        packet << "Message Terminator: END\n";

        // Generate and append checksum
        std::string packetContent = packet.str();
        packet << "Checksum: " << generateChecksum(packetContent) << "\n";

        return packet.str();
    }

    void ACARS::setTransmissionNumber(unsigned int number) {
        this->transmissionNumber = number;
    }

    void ACARS::setIsPriority(bool priority) {
        this->isPriority = priority;
    }

    void ACARS::setFlag(ACARSFlag flag) {
        this->flag = flag;
    }

    void ACARS::setAircraftID(const std::string& id) {
        this->aircraftID = id;
    }

    void ACARS::setGroundStationID(const std::string& id) {
        this->groundStationID = id;
    }

    void ACARS::setFlightInformation(const std::string& flightNumber, const std::string& aircraftType,
        const std::string& departureAirport, const std::string& destinationAirport) {
        this->flightNumber = flightNumber;
        this->aircraftType = aircraftType;
        this->departureAirport = departureAirport;
        this->destinationAirport = destinationAirport;
    }

    void ACARS::setTelemetryData(float latitude, float longitude, float altitude,
        float speed, unsigned int heading) {
        this->latitude = latitude;
        this->longitude = longitude;
        this->altitude = altitude;
        this->speed = speed;
        this->heading = heading;
    }

    std::string ACARS::getFormattedTimestamp() {
        std::string timestamp;
        std::time_t t = std::time(nullptr);
        std::tm tm;
        errno_t localtimeResult = localtime_s(&tm, &t);
        std::ostringstream timestampStream;

        if (localtimeResult == 0) {  // If no error
            timestampStream << std::setfill('0')
                << std::setw(4) << tm.tm_year + 1900 << "-"
                << std::setw(2) << tm.tm_mon + 1 << "-"
                << std::setw(2) << tm.tm_mday << " "
                << std::setw(2) << tm.tm_hour << ":"
                << std::setw(2) << tm.tm_min << ":"
                << std::setw(2) << tm.tm_sec;
            timestamp = timestampStream.str();
        }
        else {
            logs::logger.log("Error getting the timestamp.", logs::Logger::LogLevel::Error);
        }

        return timestamp; 
    }

}