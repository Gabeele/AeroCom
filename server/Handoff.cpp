#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "communication_system.h"
#include "ACARS.h"
#include "logger.h"

namespace GroundControl {


    std::string Handoff::Serialize() const {
        std::stringstream ss;
        ss << "Flag: " << Flag 
            << " Frequency: " << Frequency 
            << " Channel: " << Channel ;
        return ss.str();
    }

}