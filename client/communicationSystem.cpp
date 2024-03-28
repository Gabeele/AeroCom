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
        frequency.sin_port = htons(static_cast<u_short>(this->channel));

    }

    CommunicationSystem::~CommunicationSystem() {
        if (disconnect()) {
            logs::logger.log("Destructing the communicaiton system.", logs::Logger::LogLevel::Info);
        }

        if (WSACleanup() == SOCKET_ERROR) {
            logs::logger.log("WSA Cleanup resulted in an error.", logs::Logger::LogLevel::Error);
        }
    }

    bool CommunicationSystem::disconnect() {
        bool success = false; 

        if (socketFD != INVALID_SOCKET) {

            if (closesocket(socketFD) == SOCKET_ERROR) {

                logs::logger.log("Failed to close socket.", logs::Logger::LogLevel::Error);
            }
            else {

                logs::logger.log("Disconnected from server.", logs::Logger::LogLevel::Info);
                success = true;
            }
            socketFD = INVALID_SOCKET; 
        }

        return success;
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

    std::string CommunicationSystem::receiveMessage() {
        constexpr std::size_t bufferSize = 1024;
        char buffer[bufferSize] = { 0 };
        std::string receivedMessage; 
        int bytesReceived = recv(socketFD, buffer, bufferSize, 0);

        if (bytesReceived > 0) {

            receivedMessage = std::string(buffer, static_cast<std::string::size_type>(bytesReceived));
            logs::logger.log("Received message: " + receivedMessage, logs::Logger::LogLevel::Info);
        }
        else if (bytesReceived == 0) {
            logs::logger.log("Connection closed by the remote host.", logs::Logger::LogLevel::Warning);
        }
        else {

            int recvError = WSAGetLastError();
            logs::logger.log("Receive failed with error code: " + std::to_string(recvError), logs::Logger::LogLevel::Error);
        }

        return receivedMessage;
    }


    bool CommunicationSystem::sendFile(const std::string& path) {
        bool success = true;

        // Open the file in binary mode
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            logs::logger.log("Failed to open file: " + path, logs::Logger::LogLevel::Error);
            success = false;
        }

        size_t fileSize = 0;
        if (success) {
            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            if (fileSize == static_cast<size_t>(-1)) {
                logs::logger.log("Failed to tell the file size.", logs::Logger::LogLevel::Error);
                success = false;
            }
            file.seekg(0, std::ios::beg); 
        }

        // Send the file size to the server
        if (success) {
            std::string fileSizeStr = "Size: " + std::to_string(fileSize) + "\n";
            if (!sendMessage(fileSizeStr.c_str(), fileSizeStr.size())) {
                logs::logger.log("Failed to send file size.", logs::Logger::LogLevel::Error);
                success = false;
            }
        }

        // Send the actual file content
        if (success) {
            char buffer[1024];
            while (success && file.read(buffer, sizeof(buffer)).gcount() > 0) {
                size_t bytesToWrite = file.gcount();
                if (!sendMessage(buffer, bytesToWrite)) {
                    logs::logger.log("Failed to send file data.", logs::Logger::LogLevel::Error);
                    success = false; 
                }
            }
        }

        // Wait for an acknowledgment from the server
        if (success) {
            std::string ackMsg = receiveMessage();
            if (ackMsg != "akn") { 
                logs::logger.log("Acknowledgment not received or not as expected.", logs::Logger::LogLevel::Warning);
                success = false;
            }
            else {
                logs::logger.log("File transmission acknowledged.", logs::Logger::LogLevel::Info);
            }
        }

        return success;
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

   bool CommunicationSystem::sendMessage(const char* data, size_t size) {
       bool success = true; 

       int bytesSent = send(this->socketFD, data, static_cast<int>(size), 0);
       if (bytesSent == SOCKET_ERROR) {
           logs::logger.log("Failed to send message.", logs::Logger::LogLevel::Error);
           success = false; 
       }
       else {
           logs::logger.log("Message sent successfully.", logs::Logger::LogLevel::Info);
       }

       return success; 
   }


    bool CommunicationSystem::sectorHandoff() {

        return true; 
    }



    void CommunicationSystem::setCommunicationType(CommunicationType type) {
        this->channel = type;
    }

}