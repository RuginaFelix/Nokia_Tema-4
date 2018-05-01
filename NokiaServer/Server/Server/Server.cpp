#include "Server.hpp"

static Server* server;

Server::Server(int PORT, bool BroadcastPublically)
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically)
		addr_.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr_.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr_.sin_port = htons(PORT);
	addr_.sin_family = AF_INET;

	sListen_ = socket(AF_INET, SOCK_STREAM, NULL);

	if (bind(sListen_, (SOCKADDR*)&addr_, sizeof(addr_)) == SOCKET_ERROR)
	{
		std::string errorMessage = "Failed to bind the addres to our listening socket. Winsock Error: "
			+ std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (listen(sListen_, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string errorMessage = "Failed to listen on listening socket. Winsock Error: "
			+ std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	server = this;
}

bool Server::listenForNewConnection()
{
	SOCKET newConnection;

	newConnection = accept(sListen_, (SOCKADDR*)&addr_, &addrLength_);
	if (newConnection == 0)
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else
	{
		std::cout << "Client Connected!" << std::endl;
		connections_[totalConnections_] = newConnection;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(totalConnections_), NULL, NULL);
		std::string message = "Welcome to the server!";
		SendString(totalConnections_, message);
		++totalConnections_;
		return true;
	}
}

bool Server::ProccesPacket(int ID, Packet packetType)
{
	switch (packetType)
	{
	case P_ChatMessage:
	{
		std::string Message;
		if (!GetString(ID, Message))
			return false;

		for (int i = 0; i < totalConnections_; ++i)
		{
			if (i == ID)
				continue;
			if (!SendString(i, Message))
			{
				std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}
	default:
		std::cout << "Unrecognized packet: " << packetType << std::endl;
		break;
	}
	return true;
}

void Server::ClientHandlerThread(int ID)
{
	Packet PacketType;
	while (true)
	{
		if (!server->GetPacketType(ID, PacketType))
			break;
		if (!server->ProccesPacket(ID, PacketType))
			break;
	}

	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(server->connections_[ID]);
}