#include "communicationSystem.h"


namespace aircraft {

#pragma comment(lib, "ws2_32.lib")


    CommunicationSystem::CommunicationSystem() : socketFD(INVALID_SOCKET) {

        setChannel(DEFAULT_CHANNEL_STR);    // Sets the default channel and frequency
        setFrequency(DEFAULT_FREQUENCY);
        
        
        // Initalizes windows sockets

        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (result != 0) {
            logs::logger.log("WSA Startup has an issue. Result is non-zero.", logs::Logger::LogLevel::Error);
        }

        frequency.sin_family = AF_INET;
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

        // Windows sockets connection initalization

        socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socketFD == INVALID_SOCKET) {
            logs::logger.log("Error opening the socket.", logs::Logger::LogLevel::Error);
        }
        else {
            // Attempt to connect
            if (::connect(socketFD, reinterpret_cast<struct sockaddr*>(&frequency), sizeof(frequency)) == SOCKET_ERROR) {
                logs::logger.log("Failed to connect to the server.", logs::Logger::LogLevel::Error);
                success = false;
            }
            else {
                logs::logger.log("Connected to the server.", logs::Logger::LogLevel::Info);
                success = true;
            }

            if (!success) {
                if (closesocket(socketFD) == SOCKET_ERROR) {
                    int closeError = WSAGetLastError();
                    logs::logger.log("closesocket failed with error code: " + std::to_string(closeError), logs::Logger::LogLevel::Error);
                    success = false;
                }

                if (WSACleanup() == SOCKET_ERROR) {
                    int wsacleanupError = WSAGetLastError();
                    logs::logger.log("WSACleanup failed with error code: " + std::to_string(wsacleanupError), logs::Logger::LogLevel::Error);
                    success = false;
                }
            }

            if (intializeCommunication()) {     // Execute the initalization step
                success = true;
            }
            else {
                success = false;
            }
        }

        return success;
    }

    bool CommunicationSystem::intializeCommunication() {
        bool success = false;
        const std::string aknowledgeFlag = "Flag: A";

        // Send the trajectory to the connected server
        if (sendFile(TRAJECTORY_PATH)) {
            logs::logger.log("Large file sent.", logs::Logger::LogLevel::Info);
            success = true;
        }

        // Receive an aknowledgement packet
        if (success) {
            std::string ackMsg = receiveMessage();

            if (ackMsg.find(aknowledgeFlag) == std::string::npos) {

                logs::logger.log("Acknowledgment not received or not as expected.", logs::Logger::LogLevel::Warning);
                success = false;
            }
            else {
                logs::logger.log("File transmission acknowledged.", logs::Logger::LogLevel::Info);
            }
        }

        return success;
    }

    void CommunicationSystem::setFrequency(const std::string& ipAddress) {
        frequency.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    }

    std::string CommunicationSystem::receiveMessage() {

        const std::size_t bufferSize = BUFFER_SIZE;
        char buffer[bufferSize] = { 0 }; 
        char* bufferPtr = buffer; 
        std::string receivedMessage;

        int bytesReceived = recv(socketFD, bufferPtr, bufferSize, 0);   // Receive from the socket

        if (bytesReceived > 0) {
            receivedMessage = std::string(bufferPtr, static_cast<std::string::size_type>(bytesReceived));
            logs::logger.log("Received message: " + receivedMessage, logs::Logger::LogLevel::Info);
        }
        else {
            if (bytesReceived == 0) {
                logs::logger.log("Connection closed by the remote host.", logs::Logger::LogLevel::Warning);
                receivedMessage = "";

            }
            else {
                int recvError = WSAGetLastError();
                logs::logger.log("Receive failed with error code: " + std::to_string(recvError), logs::Logger::LogLevel::Error);
                receivedMessage = "";
            }
        }

        return receivedMessage;
    }

    bool CommunicationSystem::reconnect() {
        const unsigned int MAX_ATTEMPTS = 5;
        bool isConnected = false;
        unsigned int attemptCount = 0;

        // While the system isn't connected and attempts is less than 5, it will disconnect from the current server (Cleaning up socket) and attemp to connect again
        while (!isConnected && attemptCount < MAX_ATTEMPTS) {
            logs::logger.log("Attempting to reconnect, attempt " + std::to_string(attemptCount + 1), logs::Logger::LogLevel::Warning);

            if (disconnect()) {
                isConnected = connect();
            }

            if (!isConnected) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }

            ++attemptCount;
        }

        if (!isConnected) {
            logs::logger.log("Failed to reconnect after " + std::to_string(MAX_ATTEMPTS) + " attempts.", logs::Logger::LogLevel::Error);
        }
        else {
            logs::logger.log("Reconnected to the server successfully.", logs::Logger::LogLevel::Info);
        }

        return isConnected; 
    }


    bool CommunicationSystem::sendFile(const std::string& path) {
        bool success = true;

        std::ifstream file(path, std::ios::binary);
        size_t fileSize = 0;

        if (!file.is_open()) {
            logs::logger.log("Failed to open file: " + path, logs::Logger::LogLevel::Error);
            success = false;
        }

        if (success) {
            // Check the return value of seekg to ensure it succeeds
            if (!(file.seekg(0, std::ios::end))) {
                logs::logger.log("Failed to seek to the end of the file.", logs::Logger::LogLevel::Error);
                success = false;
            }
            else {
                fileSize = file.tellg();
                if (fileSize == static_cast<size_t>(-1)) {
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
            char buffer[BUFFER_SIZE]; 
            char* bufferPtr = buffer; 

            while (success && file.read(bufferPtr, BUFFER_SIZE) || file.gcount() > 0) {
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
           logs::logger.log("Failed to send message.", logs::Logger::LogLevel::Error);
           success = false; 
       }
       else {
           logs::logger.log("Message sent successfully.", logs::Logger::LogLevel::Info);
       }

       return success; 
   }

   bool CommunicationSystem::handoff(const std::string& newFrequency, const std::string& newChannel) {
       bool success = true; 

       std::string prevFrequency = getFrequencyString();
       CommunicationType prevChannel = this->channel;

       // Sets a new frequency and channel
       setFrequency(newFrequency);
       setChannel(newChannel);

       // Disconnects from the current server
       if (!disconnect()) {
           logs::logger.log("Failed to disconnect using current settings.", logs::Logger::LogLevel::Error);
           success = false;
       }

       // Attenpts to connect to another server, if it does then it will attempt to connect to old server
       if (!connect()) {

           logs::logger.log("Connection with new settings failed, reverting to previous settings.", logs::Logger::LogLevel::Warning);

           setFrequency(prevFrequency);
           setChannel(CommunicationTypeToString(prevChannel));

           if (!disconnect()) {
               logs::logger.log("Failed to disconnect using current settings.", logs::Logger::LogLevel::Error);
               success = false;
           }

           if (!connect()) {
              
               logs::logger.log("Reconnection with original settings also failed.", logs::Logger::LogLevel::Error);
               success = false;
           }
           else {
               
               success = true;
           }
       }
  
       return success; 
   }

    void CommunicationSystem::setChannel(const std::string& channelString) {

            this->channel = fromStringToCommunicationType(channelString);

        frequency.sin_port = htons(static_cast<u_short>(this->channel));

        logs::logger.log("Channel set to " + channelString + " with port " + CommunicationTypeToString(this->channel), logs::Logger::LogLevel::Info);
    }

    void CommunicationSystem::setCommunicationType(CommunicationType type) {
        this->channel = type;
    }

    std::string CommunicationSystem::getFrequencyString() const {
        return inet_ntoa(frequency.sin_addr);
    }



}