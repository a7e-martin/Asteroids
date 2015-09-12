#include "c_server.h"
#include <iostream>


void c_Server::Start(int port)
{
	_listener.setBlocking(false);
	_running = true;
	_listenThread = new std::thread(&c_Server::ListenForNewClients, this);

	_updateThread = new std::thread(&c_Server::Update, this);
}

c_Server::c_Server()
{

}

void c_Server::Update()
{
	while (_running)
	{
		if (_selector.wait())
		{
			for (auto& it : _clients)
			{
				if (_selector.isReady(*it.second))
				{
					sf::Packet packet;
					it.second->receive(packet);

					std::string msg;
					if (packet >> msg)
					{
						if (!msg.empty())
						{
							std::cout << msg;
							if (msg == "quit")
							{
								_running = false;
							}
						}
					}
					
				}
			}
		}
	}
	Stop();
}

void c_Server::ListenForNewClients()
{
	if (_listener.listen(5555) != sf::TcpSocket::Status::Done)
	{
		//error
	}
	while (_running)
	{
		sf::TcpSocket *newClient = new sf::TcpSocket();
		if (_listener.accept(*newClient) == sf::TcpSocket::Status::Done)
		{
			std::cout << "new client connected\n";
			_selector.add(*newClient);
			_clients.insert(std::pair<int, sf::TcpSocket*>(_clients.size(), newClient));
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}

void c_Server::JoinThread()
{
	if (_updateThread->joinable())
	{
		_updateThread->join();
	}
}

void c_Server::Stop()
{
	if (_listenThread && _listenThread->joinable())
	{
		_listenThread->join();
		delete _listenThread;
	}
}
