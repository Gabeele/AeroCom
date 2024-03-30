#include <string>
#include <sstream>
#include "logger.h"

namespace GroundControl {

    class Handoff {
    public:
        // Parameterized constructor
        Handoff(const std::string& flag, const std::string& frequency, const std::string& channel)
            : Flag(flag), Frequency(frequency), Channel(channel) {
        }
        // Getter functions
        std::string GetFlag() const;
        std::string GetFrequency() const;
        std::string GetChannel() const;

        // Serialize function to convert object data to a string
        std::string Serialize() const;

    private:
        std::string Flag;
        std::string Frequency; // Ip address of the target server
        std::string Channel; // either HF or VHF this can then be changed to enum maybe

    };
}