#pragma comment(lib,"ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <string>
#include <iostream>

const int NUMBER_OF_CONNECTIONS = 100;

enum Packet {
	P_ChatMessage
};

class Server {
public:
	Server(int, bool BroadcastPublically = false);
	bool listenForNewConnection();
private:
	SOCKET connections_[NUMBER_OF_CONNECTIONS];
	SOCKADDR_IN addr_;
	SOCKET sListen_;
	int addrLength_ = sizeof(addr_);
	int totalConnections_ = 0;

	bool SendInt(int, int);
	bool GetInt(int, int&);
	bool SendPacketType(int, Packet);
	bool GetPacketType(int, Packet&);
	bool SendString(int, std::string&);
	bool GetString(int, std::string&);
	bool ProccesPacket(int, Packet);
	static void ClientHandlerThread(int);
};