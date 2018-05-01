#include "Client.hpp"

static Client* client;

Client::Client(std::string IP, int PORT)
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr_.sin_addr.s_addr = inet_addr(IP.c_str());
	addr_.sin_port = htons(PORT);
	addr_.sin_family = AF_INET;
	client = this;
}

bool Client::Connect()
{
	connection_ = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection_, (SOCKADDR*)&addr_, sizeOfAddr_) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return true;
}

bool Client::ProcessPacket(Packet packetType)
{
	switch (packetType)
	{
	case P_ChatMessage:
	{
		std::string message;
		if (!GetString(message))
			return false;
		std::cout << message << std::endl;
		break;
	}
	default:
		std::cout << "Unrecognized packet: " << packetType << std::endl;
		break;
	}
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(connection_) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK)
			return true;

		std::string errorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void Client::ClientThread()
{
	Packet packetType;
	while (true)
	{
		if (!client->GetPacketType(packetType))
			break;
		if (!client->ProcessPacket(packetType))
			break;
	}

	std::cout << "Lost connection to the server." << std::endl;
	if (client->CloseConnection())
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	else
		std::cout << "Socket was not bale to be close." << std::endl;
}