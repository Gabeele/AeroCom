#include "communicationSystem.h"

namespace aircraft {

#pragma comment(lib, "ws2_32.lib")


    CommunicationSystem::CommunicationSystem() : socketFD(INVALID_SOCKET) {

        setCommunicationType(CommunicationType::VHF);

        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            logs::logger.log("WSA Startup has an issue. Result is non-zero.", logs::Logger::LogLevel::Error);
        }

        frequency.sin_family = AF_INET;
        frequency.sin_addr.s_addr = inet_addr(DEFAULT_FREQUENCY.c_str());
        frequency.sin_port = htons(static_cast<u_short>(channel));

    }

    CommunicationSystem::~CommunicationSystem() {
        if (socketFD != INVALID_SOCKET) {
            int result = closesocket(socketFD);
            if (result == SOCKET_ERROR) {
                logs::logger.log("Close socket resulted in a positive number.", logs::Logger::LogLevel::Error);
            }
        }
        int result = WSACleanup();
        if (result == SOCKET_ERROR) {
            logs::logger.log("WSA Cleanup resulted in a postiive number.", logs::Logger::LogLevel::Error);
        }
    }

    bool CommunicationSystem::connect() {
        bool returnFlag = false;

        socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socketFD == INVALID_SOCKET) {
            logs::logger.log("Error opening the socket.", logs::Logger::LogLevel::Error);
        }
        else {
            // Attempt to connect
            if (::connect(socketFD, reinterpret_cast<struct sockaddr*>(&frequency), sizeof(frequency)) == SOCKET_ERROR) {
                logs::logger.log("Failed to connect to the server.", logs::Logger::LogLevel::Error);
            }
            else {
                logs::logger.log("Connected to the server.", logs::Logger::LogLevel::Info);
                returnFlag = true;
            }

            if (!returnFlag) {
                if (closesocket(socketFD) == SOCKET_ERROR) {
                    int closeError = WSAGetLastError();
                    logs::logger.log("closesocket failed with error code: " + std::to_string(closeError), logs::Logger::LogLevel::Error);
                }

                if (WSACleanup() == SOCKET_ERROR) {
                    int wsacleanupError = WSAGetLastError();
                    logs::logger.log("WSACleanup failed with error code: " + std::to_string(wsacleanupError), logs::Logger::LogLevel::Error);
                }
            }
        }

        return returnFlag;
    }


    void CommunicationSystem::setFrequency(const std::string& ipAddress) {
        frequency.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    }

    bool CommunicationSystem::receiveMessage() {
        bool success = false;
        char buffer[1024];
        (void)memset(&buffer[0], 0, sizeof(buffer));

        int bytesReceived = recv(socketFD, &buffer[0], sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::string receivedMessage(buffer, bytesReceived);
            logs::logger.log("Received message: " + receivedMessage, logs::Logger::LogLevel::Info);
            success = true;
        }
        else if (bytesReceived == 0) {
            logs::logger.log("Connection closed by the remote host.", logs::Logger::LogLevel::Warning);
        }
        else {
            int recvError = WSAGetLastError();
            logs::logger.log("Receive failed with error code: " + std::to_string(recvError), logs::Logger::LogLevel::Error);
        }
        return success;
    }


    bool CommunicationSystem::sectorHandoff() {

        return true;
    }

    bool CommunicationSystem::sendMessage(const std::string& message) {
        bool returnFlag = false;

        int bytesSent = send(this->socketFD, message.c_str(), message.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            logs::logger.log("Failed to send message.", logs::Logger::LogLevel::Error);
            returnFlag = false;
        }
        else {
            logs::logger.log("Message sent successfully.", logs::Logger::LogLevel::Info);
            returnFlag = true;
        }

        return returnFlag;
    }


    void CommunicationSystem::setCommunicationType(CommunicationType type) {
        logs::logger.log("Communication Type was changed.", logs::Logger::LogLevel::Info);
        this->channel = type;
    }

}