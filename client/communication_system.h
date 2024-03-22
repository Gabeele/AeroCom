#ifndef COMMUNICATIONSYSTEM_H
#define COMMUNICATIONSYSTEM_H

#include <string>
#include <iostream>
#include <sys/types.h>
#include <winsock.h>
#include <string> 

#include "logger.h"

namespace aircraft {
    enum class CommunicationType { VHF, HF };

    class CommunicationSystem {
    public:
        CommunicationSystem();
        ~CommunicationSystem();

        bool initializeSystem(unsigned int channel);

        bool receiveMessage();
        bool sectorHandoff();
        bool sendMessage(const std::string& message);

    private:
        SOCKET socketFD;
        struct sockaddr_in channelAddress;
        logs::Logger logger;

    };
}

#endif 