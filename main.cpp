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
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, sigHandler);

	if (argc != 3)
	{
		std::cout << RED << "Error! Expected 2 arguments" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	int port;
	// TODO: Стоит разделить условие на 2 части с выводом ошибки под каждый из вариантов?
	if (!isdigit(*argv[1]) || (port = atoi(argv[1])) > SHRT_MAX)
	{
		std::cout << RED << "Error! Incorrect port number" << RESET << std::endl;
		return (EXIT_FAILURE);
	}

	Server MyServer(port, argv[2]);
	MyServer.printConfigFileFields();
	MyServer.initServer();

	while (work)
	{
		MyServer.acceptConnection();
		MyServer.messageProcessing();
	}
}
