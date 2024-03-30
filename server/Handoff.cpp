#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "communication_system.h"
#include "ACARS.h"
#include "logger.h"
#include "Handoff.h"

namespace GroundControl {

    Handoff::Handoff(const std::string& flag, const std::string& frequency, const std::string& channel)
        : Flag(flag), Frequency(frequency), Channel(channel) {
    }

    std::string Handoff::Serialize() const {
        std::stringstream ss;
        ss << "Flag: " << Flag 
            << "Frequency: " << Frequency 
            << "Channel: " << Channel ;
        return ss.str();
    }

    std::string Handoff::GetFlag() const {
        return Flag;
    }

    std::string Handoff::GetFrequency() const {
        return Frequency;
    }

    std::string Handoff::GetChannel() const {
        return Channel;
    }

}