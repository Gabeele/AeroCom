#include "communication_system.h"

namespace GroundControl {

#pragma comment(lib, "ws2_32.lib")

    GroundControl::GroundControl()
    {
    }

    GroundControl::~GroundControl() 
    {
        if (listenSocket_ != INVALID_SOCKET) 
        {
            closesocket(listenSocket_);
        }
        WSACleanup();
    }

    bool GroundControl::Initialize() 
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) 
        {
            std::cerr << "WSA Startup has an issue. Result is non-zero." << std::endl;
            return false;
        }
        return true;
    }

    bool GroundControl::Connect(int port) // connecting based on a port allows for us to specify the simualed frequency for the commuinication channel
    { 
        listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket_ == INVALID_SOCKET) 
        {
            std::cerr << "Error opening the socket." << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(static_cast<u_short>(port));

        if (bind(listenSocket_, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) 
        {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket_);
            return false;
        }

        if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) 
        {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket_);
            return false;
        }

        return true;
    }


    bool GroundControl::SendMessage(const std::string& message) 
    { // no idea what this looks like atm
        if (message == SWITCH_CHANNEL_COMMAND) 
        {
            std::string channelSwitchMessage = "Please switch to channel X"; 
            // Send the channel switch message to the client
            return true; 
        } else 
        {
            // Message is not a channel switch command, send it normally
            return false; 
        }
    }



    std::string GroundControl::ReceiveMessage() 
    {
        SOCKET clientSocket = accept(listenSocket_, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) 
        {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket_);
            WSACleanup();
            return "";
        }

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) 
        {
            std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            return "";
        }
        buffer[bytesReceived] = '\0';
        std::string receivedMessage(buffer);

        // do something with the message, ig log it

        closesocket(clientSocket);
        return receivedMessage;
    }

    void GroundControl::HandleATCToATCHandoffRequest(GroundControl* requestingServer, GroundControl* targetServer)
    {
    }

    void GroundControl::HandleATCToAircraftHandoffRequest(GroundControl* requestingServer, char* targetAircraft)
    {
    }

}
