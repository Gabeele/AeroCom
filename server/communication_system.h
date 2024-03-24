#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <iostream>
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
        bool SendMessage(const std::string& message); // might not actually be used
        std::string ReceiveMessage();

        // maybe should be broken down into a function that requests a hand off and one that accepts it
        void HandleATCToATCHandoffRequest(GroundControl* requestingServer, GroundControl* targetServer); 
        void HandleATCToAircraftHandoffRequest(GroundControl* requestingServer, char* targetAircraft);
    private:
        int port_;
        SOCKET listenSocket_;
        std::vector<GroundControl*> connectedATCServers;
        std::vector<char*> connectedAircrafts; // this will be an ip that represents each of the aircrafts in flight
    };

}
#endif // SOCKET_H
