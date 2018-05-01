#include "Server.hpp"

int main()
{
	Server server(1111);

	for (int i = 0; i < NUMBER_OF_CONNECTIONS; ++i)
		server.listenForNewConnection();

	system("pause");
}