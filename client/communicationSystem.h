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
    const std::string DEFAULT_CHANNEL_STR = "VHF";

    enum class CommunicationType : u_short { VHF = 5555, HF = 4444 };

    class CommunicationSystem {
    public:
        CommunicationSystem();
        ~CommunicationSystem();

        bool disconnect();

        bool connect();
        bool intializeCommunication();
        std::string receiveMessage();
        bool handoff(const std::string& newFrequency, const std::string& newChannel);
        void setChannel(const std::string& channel);
        bool sendMessage(const std::string& message);
        bool sendMessage(const char* data, size_t size);
        void setCommunicationType(CommunicationType type);
        std::string getFrequencyString() const;
        std::string getChannelString() const;
        void setFrequency(const std::string& ipAddress);
        bool sendFile(const std::string& path);


    private:
        SOCKET socketFD;
        struct sockaddr_in frequency;
        CommunicationType channel;


    };
#endif 
}
