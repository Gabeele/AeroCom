#ifndef COMMUNICATIONSYSTEM_H
#define COMMUNICATIONSYSTEM_H

#include <string>
#include <iostream>
#include <sys/types.h>
#include <winsock.h>
#include <string> 

#include "logger.h"

namespace aircraft {

    const std::string DEFAULT_FREQUENCY = "127.0.0.1";

    enum class CommunicationType : u_short { VHF = 5555 , HF = 4444 };

    class CommunicationSystem {
    public:
        CommunicationSystem();
        ~CommunicationSystem();

        bool connect();
        bool receiveMessage();
        bool sectorHandoff();
        bool sendMessage(const std::string& message);
        void setCommunicationType(CommunicationType type);
        void setFrequency(const std::string& ipAddress);

    private:
        SOCKET socketFD;
        struct sockaddr_in frequency;
        CommunicationType channel;
        logs::Logger logger;




    };
}

#endif 