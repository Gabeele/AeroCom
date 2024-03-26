#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace GroundControl {

    const std::string DEFAULT_FREQUENCY = "127.0.0.1";
    const unsigned int DEFAULT_CHANNEL = 5555;

    enum class CommunicationType { VHF, HF };
    // maybe some sort of channel for ground control to communicate over

    class GroundControl 
    {
    public:
        GroundControl(); // constructor wit da parameters
        ~GroundControl(); // deconstructor

        bool Initialize();
        bool Connect(int port);
        std::string ReceiveMessage(SOCKET clientSocket);
        std::string generateChecksum(const std::string& packetContent) const;
        void ChecksumCheck(std::string receivedMessage);
        SOCKET AcceptConnection();

        int GetPort() const;

        // maybe should be broken down into a function that requests a hand off and one that accepts it
        // /03/24 edit,i have no idea what i was thinking above
        void HandleATCToAircraftHandoffRequest(GroundControl* targetServer, char* targetAircraft);

        bool SwitchFrequency(int newPort);
    private:
        int port_;
        char* ip;
        SOCKET listenSocket_;
        std::vector<GroundControl*> connectedATCServers;
        std::vector<char*> connectedAircrafts; // this will be an ip that represents each of the aircrafts in flight
    };

}
#endif // SOCKET_H
