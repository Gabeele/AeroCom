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
    const unsigned int DEFAULT_CHANNEL = 5555;

    enum class CommunicationType { VHF, HF };

    class CommunicationSystem {
    public:
        CommunicationSystem();
        ~CommunicationSystem();

        bool connect();
        bool receiveMessage();
        bool sectorHandoff();
        bool sendMessage(const std::string& message);
        void setChannel(unsigned int channel);
        void setCommunicationType(CommunicationType type);
        void setFrequency(const std::string& ipAddress);

    private:
        SOCKET socketFD;
        struct sockaddr_in frequency;
        unsigned int channel;
        CommunicationType type;
        logs::Logger logger;




    };
}

#endif 