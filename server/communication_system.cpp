#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "communication_system.h"
#include "ACARS.h"
#include "logger.h"

namespace GroundControl {

#pragma comment(lib, "ws2_32.lib")

    GroundControl::GroundControl() :
        state(ServerState::Closed)
    {
    }

    GroundControl::~GroundControl()
    {
        updateServerState(ServerState::Disconnected);
        if (listenSocket_ != INVALID_SOCKET)
        {
            (void)closesocket(listenSocket_);
        }
        (void)WSACleanup();
    }

    bool GroundControl::Initialize()
    {
        WSADATA wsaData;
        updateServerState(ServerState::Idle);
        bool ret = true;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            logs::logger.log("WSA Startup has an issue. Result is non-zero.", logs::Logger::LogLevel::Error);
            ret = false;
        }
        return ret;
    }

    bool GroundControl::Connect(int port) // connecting based on a port allows for us to specify the simualed frequency for the commuinication channel
    {
        bool ret = true;
        updateServerState(ServerState::Idle);
        listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket_ == INVALID_SOCKET)
        {
            logs::logger.log("Error opening the socket.", logs::Logger::LogLevel::Error);
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(DEFAULT_FREQUENCY.c_str());
        serverAddr.sin_port = htons(static_cast<u_short>(port));

        if (bind(listenSocket_, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        {
            //std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            (void)closesocket(listenSocket_);
            return false;
        }

        if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
        {
            //std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            (void)closesocket(listenSocket_);
            return false;
        }

        return ret;
    }

    bool GroundControl::SwitchFrequency(int newPort) // should take a port number which can be changed to the HF or VHF enum to change between port 4444 or 5555 this can also be an enum probably
    {
        bool ret = true;
        SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (newSocket == INVALID_SOCKET)
        {
            logs::logger.log("Error creating socket for new frequency.", logs::Logger::LogLevel::Error);
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(DEFAULT_FREQUENCY.c_str());;
        serverAddr.sin_port = htons(static_cast<u_short>(newPort));


        // binds new socket and listens for new connection without dropping until it connects
        if (bind(newSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            (void)closesocket(newSocket);
            return false;
        }

        if (listen(newSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            (void)closesocket(newSocket);
            return false;
        }

        if (listenSocket_ != INVALID_SOCKET)
        {
            (void)closesocket(listenSocket_);
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

    bool GroundControl::ReceiveMessage(SOCKET clientSocket)
    {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            return false;
        }
        else if (bytesReceived == 0)
        {
            return false;
        }

        // check if the incoming file is an image or packet
        // for image, search keyword "Filesize:"
        // else it's a packet

        std::string receivedMessage(buffer);
        if (strstr(buffer, "Filesize:"))
        {
            std::ofstream outfile("recv_trajectory.png", std::ios::binary);

            char imgbuffer[1024];
            int inc_bytes;

            do {
                inc_bytes = recv(clientSocket, imgbuffer, sizeof(imgbuffer), 0);
                if (strstr(imgbuffer, "EOF")) {
                    outfile.close();

                    logs::logger.log("Trajectory received successfully", logs::Logger::LogLevel::Info);

                    send(clientSocket, "Flag: A", sizeof("Flag: A"), 0);
                    return true;
                }
                else if (inc_bytes > 0) {
                    outfile.write(imgbuffer, inc_bytes);
                }
                else {
                    std::cerr << "Receiving failed with error: " << WSAGetLastError() << std::endl;
                }
            } while (inc_bytes > 0);
        }
        else
        {
            buffer[bytesReceived] = '\0';

            PacketParsing(receivedMessage);

            //check checksum integrity
            ChecksumCheck(receivedMessage);

            return true;
        }
    }

    void GroundControl::PacketParsing(std::string msg)
    {
        // finds new line character and skips the first 2 lines of the packet
        int nl = msg.find('\n');
        nl = msg.find('\n', nl + 1);
        nl = msg.find('\n', nl + 1);

        unsigned int tnum = atoi(msg.substr(msg.find("Transmission Number: ") + 21, nl - (msg.find("Transmission Number: ") + 21)).c_str());
        nl = msg.find('\n', nl + 1);
        
        bool priority = false;
        if (strcmp((msg.substr(msg.find("Message Priority: ") + 18, nl - (msg.find("Message Priority: ") + 18))).c_str(), "Yes") == 0)
        {
            priority = true;
        }
        nl = msg.find('\n', nl + 1);

        aircraft::ACARSFlag flg;
        switch (msg.substr(msg.find("Flag: ") + 6, msg.find("Flag: ") + 7)[0]) {
        case 'H':
            flg = aircraft::ACARSFlag::Handoff;
            break;
        case 'D':
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
        nl = msg.find('\n', nl + 1);
        std::string acID = msg.substr(msg.find("Aircraft ID: ") + 13, nl - (msg.find("Aircraft ID: ") + 13));
        nl = msg.find('\n', nl + 1);

        std::string gsID = msg.substr(msg.find("Ground Station ID:") + 18, nl - (msg.find("Ground Station ID:") + 18));
        nl = msg.find('\n', nl + 1);
        nl = msg.find('\n', nl + 1);

        //flight information
        std::string flNum = msg.substr(msg.find("Flight Number: ") + 15, nl - (msg.find("Flight Number: ") + 15));
        nl = msg.find('\n', nl + 1);
        std::string acType = msg.substr(msg.find("Aircraft Type: ") + 15, nl - (msg.find("Aircraft Type: ") + 15));
        nl = msg.find('\n', nl + 1);
        std::string depAirport = msg.substr(msg.find("Departure Airport: ") + 19, nl - (msg.find("Departure Airport: ") + 19));
        nl = msg.find('\n', nl + 1);
        std::string destAirport = msg.substr(msg.find("Destination Airport: ") + 21, nl - (msg.find("Destination Airport: ") + 21));
        nl = msg.find('\n', nl + 1);
        nl = msg.find('\n', nl + 1);

        // location details
        std::string lat = msg.substr(msg.find("Latitude: ") + 10, nl - (msg.find("Latitude: ") + 10));
        nl = msg.find('\n', nl + 1);
        std::string lon = msg.substr(msg.find("Longitude: ") + 11, nl - (msg.find("Longitude: ") + 11));
        nl = msg.find('\n', nl + 1);
        std::string alt = msg.substr(msg.find("Altitude: ") + 10, nl - (msg.find("Altitude: ") + 10));
        nl = msg.find('\n', nl + 1);
        std::string speed = msg.substr(msg.find("Speed: ") + 7, nl - (msg.find("Speed: ") + 7));
        nl = msg.find('\n', nl + 1);
        std::string heading = msg.substr(msg.find("Heading: ") + 9, nl - (msg.find("Heading: ") + 9));
        nl = msg.find('\n', nl + 1);

        std::string tmstmp = msg.substr(msg.find("Time Stamp:") + 11, nl - (msg.find("Time Stamp:") + 11));

        logs::logger.log("Packet " + std::to_string(tnum) + " received from " + acID + 
            "\n\tDeparture: " + depAirport + "\n\tDestination: " + destAirport,
            logs::Logger::LogLevel::Info);

        aircraft::ACARS newAcars();
    }

    void GroundControl::HandleATCToAircraftHandoffRequest(GroundControl* targetServer, char* targetAircraft)
    {
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
            logs::logger.log("Lost packet info", logs::Logger::LogLevel::Error);

        }
    }

    void GroundControl::updateServerState(ServerState newState) {

        if (newState == state)
        {
            return;
        }

        // Convert the newState enum to a descriptive string
        std::string newStateStr;
        switch (newState) {
        case ServerState::Idle:
            newStateStr = "Idle";
            break;
        case ServerState::Closed:
            newStateStr = "Closed";
            break;
        case ServerState::Connected:
            newStateStr = "Connected";
            break;
        case ServerState::Handoff:
            newStateStr = "Handoff";
            break;
        case ServerState::Disconnected:
            newStateStr = "Disconnected";
            break;
        default:
            newStateStr = "Unknown State";
            break;
        }

        logs::logger.log("Updated Server state to: " + newStateStr, logs::Logger::LogLevel::Info);


        state = newState;
    }

    int GroundControl::GetPort() const
    {
        return port_;
    }

    void GroundControl::HandleATCToAircraftHandoffRequest(SOCKET CurrentGroundToAir, Handoff* targetServer)
    {
        if (targetServer == nullptr) {
            logs::logger.log("Invalid servers for handoff request.", logs::Logger::LogLevel::Error);
            return;
        }

            if (CurrentGroundToAir == INVALID_SOCKET)
            {
                logs::logger.log("Error creating socket for aircraft.", logs::Logger::LogLevel::Error);
                return;
            }

            std::string packet = targetServer->Serialize();

            if (send(CurrentGroundToAir, packet.c_str(), packet.length(), 0) == SOCKET_ERROR)
            {
                std::cerr << "Failed to send handoff message to aircraft." << std::endl;
            }
            else 
            {
                std::cout << "Handoff message sent to aircraft successfully." << std::endl;
            }
            updateServerState(ServerState::Handoff);
            closesocket(CurrentGroundToAir);
    }


}