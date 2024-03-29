#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "communication_system.h"
#include "ACARS.h"
#include "logger.h"

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

    bool GroundControl::SwitchFrequency(int newPort) // should take a port number which can be changed to the HF or VHF enum to change between port 4444 or 5555
    {
        SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (newSocket == INVALID_SOCKET) 
        {
            std::cerr << "Error creating socket for new frequency." << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(static_cast<u_short>(newPort));


        // binds new socket and listens for new connection without dropping until it connects
        if (bind(newSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) 
        {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(newSocket);
            return false;
        }

        if (listen(newSocket, SOMAXCONN) == SOCKET_ERROR) 
        {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(newSocket);
            return false;
        }

        if (listenSocket_ != INVALID_SOCKET) 
        {
            closesocket(listenSocket_);
        }

        // Assign the new socket to the client socket member variable
        listenSocket_ = newSocket;

        return true;
    }

    SOCKET GroundControl::AcceptConnection()
    {
        SOCKET clientSocket = accept(listenSocket_, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket_);
            WSACleanup();
            throw std::exception("Cant accept client connection");
        }
        return clientSocket;
    }

    std::string GroundControl::ReceiveMessage(SOCKET clientSocket)
    {
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

        // do something with the message, ig log it or parse for whatever use it has
        // if from other GC get we can get ip and then tell the 

        // just couts the packet for now
        PacketParsing(receivedMessage);

        //check checksum integrity
        ChecksumCheck(receivedMessage);
        
        //closesocket(clientSocket);
        return receivedMessage;
    }

    void GroundControl::PacketParsing(std::string receivedMessage)
    {
        std::cout << receivedMessage << std::endl;

        unsigned int tnum = atoi(receivedMessage.substr(33, 33 + sizeof(int)).c_str());
        bool priority = false;
        if (receivedMessage.substr(52, receivedMessage.find('\n', 52)) == "Yes")
            priority = true;
        aircraft::ACARSFlag flg;
        switch (receivedMessage.substr(receivedMessage.find("Flag: ") + 6, receivedMessage.find("Flag: ") + 7)[0]) {
        case 'H':
            flg = aircraft::ACARSFlag::Handoff;
            break;
        case 'D' :
            flg = aircraft::ACARSFlag::Data;
            break;
        case 'A':
            flg = aircraft::ACARSFlag::Acknowledge;
            break;
        case 'M':
            flg = aircraft::ACARSFlag::Message;
            break;
        case 'R':
            flg = aircraft::ACARSFlag::Request;
            break;
        }
        std::string acID = receivedMessage.substr(receivedMessage.find("Aircraft ID: ") + 13, receivedMessage.find('\n'));
        //this is not in the transmission right now
        //std::string gsID = receivedMessage.substr(receivedMessage.find(""));

        //flight information
        std::string flNum = receivedMessage.substr(receivedMessage.find("Flight Number: ") + 15, receivedMessage.find('\n'));
        std::string acType = receivedMessage.substr(receivedMessage.find("Aircraft Type: ") + 15, receivedMessage.find('\n'));
        std::string depAirport = receivedMessage.substr(receivedMessage.find("Departure Airport: ") + 19, receivedMessage.find('\n'));
        std::string destAirport = receivedMessage.substr(receivedMessage.find("Destination Airport: ") + 21, receivedMessage.find('\n'));
        
        //rn there's an issue cutting off at the wrong place
        // finish the rest of the parsing ...


        aircraft::ACARS newAcars();
    }

    std::string GroundControl::generateChecksum(const std::string& packetContent) const {
        unsigned int checksum = 0;
        for (char c : packetContent) {
            checksum += static_cast<unsigned int>(c);
        }
        std::stringstream ss;
        ss << std::hex << checksum;
        return ss.str();
    }

    void GroundControl::ChecksumCheck(std::string receivedMessage)
    {
        std::string recievedChecksum = generateChecksum(receivedMessage.substr(0, receivedMessage.find("Checksum:")));
        std::string packetChecksum = receivedMessage.substr(receivedMessage.find("Checksum:") + 10, receivedMessage.length());
        packetChecksum.erase(packetChecksum.length() - 1);

        if (strcmp(recievedChecksum.c_str(), packetChecksum.c_str()) != 0)
        {
            // figure out what to do if the packets dont match
            std::cout << "Lost packet info" << std::endl;
        }
    }

    int GroundControl::GetPort() const 
    {
        return port_;
    }

    void GroundControl::HandleATCToAircraftHandoffRequest(GroundControl* targetServer, char* targetAircraft)
    {

        if (targetServer == nullptr) {
            std::cerr << "Invalid servers for handoff request." << std::endl;
            return;
        }

        int newPort = targetServer->GetPort();

        if (targetServer->SwitchFrequency(newPort)) 
        {
            SOCKET aircraftSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (aircraftSocket == INVALID_SOCKET) 
            {
                std::cerr << "Error creating socket for aircraft." << std::endl;
                return;
            }

            sockaddr_in aircraftAddr;
            aircraftAddr.sin_family = AF_INET;
            aircraftAddr.sin_addr.s_addr = inet_addr(targetAircraft); 
            aircraftAddr.sin_port = htons(static_cast<u_short>(newPort));

            if (connect(aircraftSocket, reinterpret_cast<sockaddr*>(&aircraftAddr), sizeof(aircraftAddr)) == SOCKET_ERROR) 
            {
                std::cerr << "Failed to connect to aircraft." << std::endl;
                closesocket(aircraftSocket);
                return;
            }

            //// send message to the aircraft to swap, hence PACKETTHING
            //if (send(aircraftSocket, PACKETTHING.c_str(), PACKETTHING.length(), 0) == SOCKET_ERROR) 
            //{
            //    std::cerr << "Failed to send handoff message to aircraft." << std::endl;
            //}
            //else 
            //{
            //    std::cout << "Handoff message sent to aircraft successfully." << std::endl;
            //}

            closesocket(aircraftSocket);
        }
        else {
            std::cerr << "Failed to switch frequency on the requesting server." << std::endl;
        }
    }


}
