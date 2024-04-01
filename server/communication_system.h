#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "Handoff.h"

namespace GroundControl {

    const std::string DEFAULT_FREQUENCY = "127.0.0.1";

    const unsigned int DEFAULT_CHANNEL = 5555;

    enum class CommunicationType { VHF, HF };
    enum class ServerState { Closed, Idle, Connected, Handoff, Disconnected };
    // maybe some sort of channel for ground control to communicate over

    class GroundControl
    {
    public:
        GroundControl(); // constructor wit da parameters
        ~GroundControl(); // deconstructor

        bool Initialize();
        bool Connect(int port);
        bool ReceiveMessage(SOCKET clientSocket);
        std::string generateChecksum(const std::string& packetContent) const;
        void PacketParsing(std::string recievedMessage);
        void ChecksumCheck(std::string receivedMessage);
        void updateServerState(ServerState newState);
        void HandleATCToAircraftHandoffRequest(GroundControl* targetServer, char* targetAircraft);
        SOCKET AcceptConnection();

        int GetPort() const;

        // maybe should be broken down into a function that requests a hand off and one that accepts it
        // /03/24 edit,i have no idea what i was thinking above
        void HandleATCToAircraftHandoffRequest(SOCKET CurrentGroundToAir, Handoff* targetServer);

        bool SwitchFrequency(int newPort);
    private:
        int port_;
        char* ip;
        SOCKET listenSocket_;
        ServerState state;
        std::vector<GroundControl*> connectedATCServers;
        std::vector<char*> connectedAircrafts; // this will be an ip that represents each of the aircrafts in flight
    };

}
#endif // SOCKET_H