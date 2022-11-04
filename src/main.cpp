#include "Utils.hpp"
#include "Server.hpp"

bool	work = true;

void	sigHandler(int signum)
{
	(void)signum;
	work = false;
}

int main(int argc, char *argv[])
{
	// TODO: Можно убрать если юзать IRC_NOSIGNAL в send. Надо проверить
//	signal(SIGPIPE, SIG_IGN);

	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	if (argc != 3)
	{
		std::cout << RED << "Error! Expected 2 arguments" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	int port;
	if (!isdigit(*argv[1]) || (port = atoi(argv[1])) > SHRT_MAX)
	{
		std::cout << RED << "Error! Incorrect port number" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	Server MyServer(port, argv[2]);
	MyServer.initServer();
	MyServer.initCommands();

	while (work)
	{
		MyServer.acceptConnection();
		MyServer.messageProcessing();
		MyServer.deleteUsersFromServer();
		MyServer.deleteEmptyChannels();
	}

	return 0;
}
