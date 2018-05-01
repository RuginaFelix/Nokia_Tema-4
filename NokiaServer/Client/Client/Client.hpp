#pragma comment(lib,"ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet {
	P_ChatMessage
};

class Client {
public:
	Client(std::string, int);
	bool Connect();
	bool CloseConnection();
	bool SendString(std::string&);
private:
	SOCKET connection_;
	SOCKADDR_IN addr_;
	int sizeOfAddr_ = sizeof(addr_);

	bool ProcessPacket(Packet);
	static void ClientThread();
	bool SendInt(int);
	bool SendPacketType(Packet);
	bool GetInt(int&);
	bool GetPacketType(Packet&);
	bool GetString(std::string&);
};