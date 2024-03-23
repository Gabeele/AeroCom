#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h> 
#include <iostream>
#include <string>

namespace GroundControl {

    const std::string DEFAULT_FREQUENCY = "127.0.0.1";
    const unsigned int DEFAULT_CHANNEL = 5555;

    enum class CommunicationType { VHF, HF };

    class GroundControl 
    {
    public:
        GroundControl(int port); // constructor wit da parameters
        ~GroundControl(); // deconstructor
        bool Initialize();
        bool Connect();
        bool SendMessage(const std::string& message);
        std::string ReceiveMessage();
    private:
        int port_;
        SOCKET listenSocket_;
    };

}
#endif // SOCKET_H
