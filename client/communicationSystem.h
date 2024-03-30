// File to denote the communication functions and properties
#pragma once 

#include <string>
#include <iostream>
#include <sys/types.h>
#include <winsock.h>
#include <string> 

#include "logger.h"
#include "Utilities.h"

namespace aircraft {

    class CommunicationSystem {
    public:

        CommunicationSystem();
        ~CommunicationSystem();

        /// <summary>
        /// Disconnects the current communication session.
        /// </summary>
        /// <returns>True if successfully disconnected; false otherwise.</returns>
        bool disconnect();

        /// <summary>
        /// Establishes a new communication session.
        /// </summary>
        /// <returns>True if the connection is successfully established; false otherwise.</returns>
        bool connect();

        /// <summary>
        /// Initializes the communication process, possibly by setting up initial conditions or sending a handshake message.
        /// </summary>
        /// <returns>True if communication is successfully initialized; false otherwise.</returns>
        bool intializeCommunication();

        /// <summary>
        /// Receives a message from the communication channel.
        /// </summary>
        /// <returns>A string containing the received message.</returns>
        std::string receiveMessage();

        /// <summary>
        /// Hands off the communication to a new frequency and channel.
        /// </summary>
        /// <param name="newFrequency">The new frequency to switch to.</param>
        /// <param name="newChannel">The new channel to switch to.</param>
        /// <returns>True if the handoff is successful; false otherwise.</returns>
        bool handoff(const std::string& newFrequency, const std::string& newChannel);

        /// <summary>
        /// Sets the communication channel of the system.
        /// </summary>
        /// <param name="channel">The channel to be set for communication.</param>
        void setChannel(const std::string& channel);

        /// <summary>
        /// Sends a string message over the communication channel.
        /// </summary>
        /// <param name="message">The message to send.</param>
        /// <returns>True if the message is sent successfully; false otherwise.</returns>
        bool sendMessage(const std::string& message);

        /// <summary>
        /// Sends binary data over the communication channel.
        /// </summary>
        /// <param name="data">A pointer to the binary data to send.</param>
        /// <param name="size">The size of the data in bytes.</param>
        /// <returns>True if the data is sent successfully; false otherwise.</returns>
        bool sendMessage(const char* data, size_t size);

        /// <summary>
        /// Sets the communication type (VHF or HF) for the system.
        /// </summary>
        /// <param name="type">The communication type to set.</param>
        void setCommunicationType(CommunicationType type);

        /// <summary>
        /// Retrieves the current frequency as a string.
        /// </summary>
        /// <returns>The frequency as a string.</returns>
        std::string getFrequencyString() const;

        /// <summary>
        /// Sets the communication frequency of the system.
        /// </summary>
        /// <param name="ipAddress">The IP address to be set for the frequency.</param>
        void setFrequency(const std::string& ipAddress);

        /// <summary>
        /// Sends a file over the communication channel.
        /// </summary>
        /// <param name="path">The file path to the file that needs to be sent.</param>
        /// <returns>True if the file is sent successfully; false otherwise.</returns>
        bool sendFile(const std::string& path);


    private:
        SOCKET socketFD;                // The full socket
        struct sockaddr_in frequency;   // The frequency/IP of the current server
        CommunicationType channel;      // The channel VHF/HF (Port)

    };
}
