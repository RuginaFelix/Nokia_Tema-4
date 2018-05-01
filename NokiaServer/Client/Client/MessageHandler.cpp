#include "Client.hpp"

bool Client::SendInt(int _int)
{
	int RetnCheck = send(connection_, (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::GetInt(int & _int)
{
	int RetnCheck = recv(connection_, (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::SendPacketType(Packet packetType)
{
	int RetnCheck = send(connection_, (char*)&packetType, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::GetPacketType(Packet& packetType)
{
	int RetnCheck = recv(connection_, (char*)&packetType, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::SendString(std::string& _string)
{
	if (!SendPacketType(P_ChatMessage))
		return false;

	int bufferLength = _string.size();
	if (!SendInt(bufferLength))
		return false;

	int RetnCheck = send(connection_, _string.c_str(), bufferLength, NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::GetString(std::string& _string)
{
	int bufferLength;
	if (!GetInt(bufferLength))
		return false;

	char* buffer = new char[bufferLength + 1];
	buffer[bufferLength] = '\0';
	int RetnCheck = recv(connection_, buffer, bufferLength, NULL);
	_string = buffer;
	delete[] buffer;

	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}