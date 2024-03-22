#include "communication_system.h"

namespace aircraft {

#pragma comment(lib, "ws2_32.lib")

    CommunicationSystem::CommunicationSystem() : socketFD(INVALID_SOCKET), logger("communications.txt", true) {
      
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            
            logger.log("WSA Startup has an issue. Result is non-zero.", logs::Logger::LogLevel::Error);
        }

        
        if (std::memset(&channelAddress, 0, sizeof(channelAddress)) != &channelAddress) {
            logger.log("Unexpected memset result.", logs::Logger::LogLevel::Error);
        }
    }

    CommunicationSystem::~CommunicationSystem() {
        if (socketFD != INVALID_SOCKET) {
            int result = closesocket(socketFD);
            if (result == SOCKET_ERROR) {
                logger.log("Close socket resulted in a positive number.", logs::Logger::LogLevel::Error);
            }
        }
        int result = WSACleanup();
        if (result == SOCKET_ERROR) {
            logger.log("WSA Cleanup resulted in a postiive number.", logs::Logger::LogLevel::Error);
        }
    }

    bool CommunicationSystem::initializeSystem(unsigned int channel) {
        bool returnFlag = true;
        socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socketFD == INVALID_SOCKET) {
            logger.log("Error opening the socket.", logs::Logger::LogLevel::Error);
            returnFlag = false;
        }

        channelAddress.sin_family = AF_INET;
        channelAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        channelAddress.sin_port = htons(static_cast<u_short>(channel));

        if (bind(socketFD, reinterpret_cast<struct sockaddr*>(&channelAddress), sizeof(channelAddress)) == SOCKET_ERROR) {
            logger.log("Binding did not execute.", logs::Logger::LogLevel::Error);
            returnFlag = false;
        }

        if (listen(socketFD, SOMAXCONN) == SOCKET_ERROR) {
            logger.log("Listening failed.", logs::Logger::LogLevel::Error);           
            returnFlag = false;
        }

        if (returnFlag == false)
        {
            int cleanup = WSACleanup();
            int close = closesocket(socketFD);

            // Checking for failures
            if (cleanup != 0 || close != 0) {
                // At least one of the operations failed
                if (cleanup != 0) {
                    int wsacleanupError = WSAGetLastError();
                    logger.log("WSACleanup failed with error code: " + std::to_string(wsacleanupError), logs::Logger::LogLevel::Error);
                }
                if (close != 0) {
                    int closeError = WSAGetLastError();
                    logger.log("closesocket failed with error code: " + std::to_string(closeError), logs::Logger::LogLevel::Error);
                }
            }

            logger.log("The socket was not able to intialize, socket did not close correctly,and the cleanup did not execute.", logs::Logger::LogLevel::Error);

        }
        else {
            logger.log("Intialized the socket connection.", logs::Logger::LogLevel::Info);
        }

        return returnFlag;
    }


    bool CommunicationSystem::receiveMessage() {

        return true; 
    }

    bool CommunicationSystem::sectorHandoff() {

        return true; 
    }

    bool CommunicationSystem::sendMessage(const std::string & message) {

        return true; 
    }

}