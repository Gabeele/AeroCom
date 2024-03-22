#include <iostream>
#include "communication_system.h"

int main() {

    // Just testing the communication system. 
    aircraft::CommunicationSystem comms;

    comms.setFrequency("127.0.0.1");
    comms.setChannel(12345);

    if (comms.connect()) {
        std::cout << "Connected to the server." << std::endl;
        if (comms.sendMessage("Hello from C++ client!")) {
            std::cout << "Message sent." << std::endl;
        }

        if (comms.receiveMessage()) {
            std::cout << "Message received." << std::endl;
        }
    }
    else {
        std::cout << "Failed to connect to the server." << std::endl;
    }

    return 0;
}
