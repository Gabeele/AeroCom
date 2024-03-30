#include <iostream>
#include "communication_system.h"

int main()
{
	GroundControl::GroundControl gc;
	SOCKET clientSocket;
	

	std::string tempFlag = "H";
	std::string tempFreq = "127.0.0.15";
	std::string tempChan = "VHF";
	GroundControl::Handoff handoff(tempFlag, tempFreq, tempChan);

	gc.Initialize();

	// first while toop keeps wsa open
	// second loops keeps recieving messages after it is connected to a client
	while (true)
	{
		if (gc.Connect(GroundControl::DEFAULT_CHANNEL))
		{
			std::cout << "hello world from the server" << std::endl;

			try
			{
				clientSocket = gc.AcceptConnection();
			}
			catch (std::exception error)
			{
				std::cout << error.what() << std::endl;
				continue;
			}

			while (true)
			{
				gc.ReceiveMessage(clientSocket);
			}
		}
		else
		{
			std::cout << "Error connecting to client" << std::endl;
		}
	}

	return 0;
}

