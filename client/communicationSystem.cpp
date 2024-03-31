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
        bool success = false; 

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
                success = true;
            }

            if (!success) {
                if (closesocket(socketFD) == SOCKET_ERROR) {
                    int closeError = WSAGetLastError();
                    logs::logger.log("closesocket failed with error code: " + std::to_string(closeError), logs::Logger::LogLevel::Error);
                }

                if (WSACleanup() == SOCKET_ERROR) {
                    int wsacleanupError = WSAGetLastError();
                    logs::logger.log("WSACleanup failed with error code: " + std::to_string(wsacleanupError), logs::Logger::LogLevel::Error);
                }
            }

            if (intializeCommunication()) { // run the first step connection process of sending a trajectory file
                success = true;
            }
        }

        return success;
    }

    bool CommunicationSystem::intializeCommunication() {
        bool success = false;

        if (sendFile("./trajectory.png")) { // TODO remove hard code
            logs::logger.log("Large file transfer is completed.", logs::Logger::LogLevel::Info);
            success = true;
        }

        return success;
    }

    void CommunicationSystem::setFrequency(const std::string& ipAddress) {
        frequency.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    }

    std::string CommunicationSystem::receiveMessage() {
        const std::size_t bufferSize = 1024;
        char buffer[bufferSize] = { 0 }; 
        char* bufferPtr = buffer; 

        std::string receivedMessage;

        int bytesReceived = recv(socketFD, bufferPtr, bufferSize, 0);

        if (bytesReceived > 0) {

            receivedMessage = std::string(bufferPtr, static_cast<std::string::size_type>(bytesReceived));
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
        // Proceed only if the file was successfully opened
        if (success) {
            // Check the return value of seekg to ensure it succeeds
            if (!(file.seekg(0, std::ios::end))) {
                logs::logger.log("Failed to seek to the end of the file.", logs::Logger::LogLevel::Error);
                success = false;
            }
            else {
                fileSize = file.tellg();
                if (fileSize == static_cast<size_t>(-1)) { // tellg() returns -1 on failure
                    logs::logger.log("Failed to tell the file size.", logs::Logger::LogLevel::Error);
                    success = false;
                }
                // Check the return value of seekg when seeking back to the beginning
                if (!(file.seekg(0, std::ios::beg))) {
                    logs::logger.log("Failed to seek back to the beginning of the file.", logs::Logger::LogLevel::Error);
                    success = false;
                }
            }
        }

        // Send the file size to the server if the file operations were successful
        if (success) {
            std::string fileSizeStr = "Filesize:" + std::to_string(fileSize) + "\n";
            if (!sendMessage(fileSizeStr)) {
                logs::logger.log("Failed to send file size.", logs::Logger::LogLevel::Error);
                success = false;
            }
        }

        // Send the file content if the file size was successfully sent
        if (success) {
            constexpr std::size_t bufferSize = 1024;
            char buffer[bufferSize]; 
            char* bufferPtr = buffer; 

            while (success && file.read(bufferPtr, bufferSize) || file.gcount() > 0) {
                size_t bytesToWrite = static_cast<size_t>(file.gcount());
                if (!sendMessage(bufferPtr, bytesToWrite)) {
                    logs::logger.log("Failed to send file data.", logs::Logger::LogLevel::Error);
                    success = false;
                    break; 
                }
            }
        }

        if (success) {
            sendMessage("EOF");
            std::string ackMsg = receiveMessage();
            if (strcmp("akn", ackMsg.c_str())) {
                logs::logger.log("Acknowledgment not received or not as expected.", logs::Logger::LogLevel::Warning);
                success = false;
            }
            else {
                logs::logger.log("File transmission acknowledged.", logs::Logger::LogLevel::Info);
            }
        }
        else {
            logs::logger.log("Skipping acknowledgment check due to previous errors.", logs::Logger::LogLevel::Info);
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
           std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
           logs::logger.log("Failed to send message.", logs::Logger::LogLevel::Error);
           success = false; 
       }
       else {
           logs::logger.log("Message sent successfully.", logs::Logger::LogLevel::Info);
       }

       return success; 
   }


    bool CommunicationSystem::handoff(const std::string freqency, const std::string channel) {

        setFrequency(freqency);
        setChannel(channel);
        disconnect();
        connect();

        return true; 
    }

    void CommunicationSystem::setChannel(const std::string& channelString) {

        if (channelString == "HF") {
            this->channel = CommunicationType::HF;
        }
        else {
            this->channel = CommunicationType::VHF;
        }

        frequency.sin_port = htons(static_cast<u_short>(this->channel));

        logs::logger.log("Channel set to " + channelString + " with port " + std::to_string(static_cast<u_short>(this->channel)), logs::Logger::LogLevel::Info);
    }



    void CommunicationSystem::setCommunicationType(CommunicationType type) {
        this->channel = type;
    }

}