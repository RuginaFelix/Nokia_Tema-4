#include "Server.hpp"

bool Server::SendInt(int ID, int _int)
{
	int RetnCheck = send(connections_[ID], (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetInt(int ID, int& _int)
{
	int RetnCheck = recv(connections_[ID], (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::SendPacketType(int ID, Packet packetType)
{
	int RetnCheck = send(connections_[ID], (char*)&packetType, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetPacketType(int ID, Packet& packetType)
{
	int RetnCheck = recv(connections_[ID], (char*)&packetType, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::SendString(int ID, std::string& _string)
{
	if (!SendPacketType(ID, P_ChatMessage))
		return false;

	int bufferLength = _string.size();
	if (!SendInt(ID, bufferLength))
		return false;

	int RetnCheck = send(connections_[ID], _string.c_str(), bufferLength, NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetString(int ID, std::string& _string)
{
	int bufferLength;
	if (!GetInt(ID, bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';
	int RetnCheck = recv(connections_[ID], buffer, bufferLength, NULL);
	_string = buffer;
	delete[] buffer;

	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}