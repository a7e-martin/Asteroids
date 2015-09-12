#ifndef DEF_C_SERVER
#define DEF_C_SERVER
#include <map>
#include <thread>
#include "SFML/Network.hpp"

class c_Server{
private:
	int _port;
	bool _running;
	sf::SocketSelector _selector;
	sf::TcpListener _listener;
	std::map<int ,sf::TcpSocket*> _clients;
	std::thread *_listenThread;
	std::thread *_updateThread;
public:
	c_Server();
	void Start(int port);
	void ListenForNewClients();
	void Update();
	void JoinThread();
	void Stop();
};
#endif