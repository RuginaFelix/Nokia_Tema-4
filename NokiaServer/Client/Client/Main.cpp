#include "Client.hpp"

int main()
{
	Client client("127.0.0.1", 1111);

	if (!client.Connect())
	{
		std::cout << "Failed to connect to server." << std::endl;
		system("pause");
		return 1;
	}

	std::string userInput;
	while (true)
	{
		std::getline(std::cin, userInput);
		if (!client.SendString(userInput))
			break;
	}
	return 0;
}