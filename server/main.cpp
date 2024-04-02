/// CSCN74000 - Software Safety and Reliability 
/// Group Project 
/// Group 1  - Lisa, Gavin, Thomas 
/// 
/// AeroCom - Simulation of auotmated handoff between two ATC systems
/// Server main
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

	// first while loop keeps wsa open
	// second loops keeps recieving messages after it is connected to a client
	while (true)
	{
		if (gc.Connect(GroundControl::DEFAULT_CHANNEL))
		{
			try
			{
				clientSocket = gc.AcceptConnection();
			}
			catch (std::exception error)
			{
				std::cout << error.what() << std::endl;
				continue;
			}

			auto startTime = std::chrono::steady_clock::now();
			gc.updateServerState(GroundControl::ServerState::Connected);
			
			while (gc.ReceiveMessage(clientSocket))
			{
				// keep track of time that client had been connected
				auto currentTime = std::chrono::steady_clock::now();
				std::chrono::duration<float> elapsed = currentTime - startTime;
				float elapsedTime = elapsed.count();
				// handoff after 60s
				if (elapsedTime >= 60.0)
				{
					// handoff the client to new ATC and close connnection
					// end loop and wait for new client to connect
					gc.HandleATCToAircraftHandoffRequest(clientSocket, &handoff);
					break;
				}
			}

			gc.updateServerState(GroundControl::ServerState::Disconnected);
		}
	}

	return 0;
}

