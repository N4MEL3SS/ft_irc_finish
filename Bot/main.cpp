#include "Client.hpp"

int main(int argc, char **argv){
	if (argc != 4){
		exit(EXIT_FAILURE);
	}
	Client client(std::atoi(argv[1]), argv[2], argv[3]);
	client.connectToPort();
	client.sendPassToServer();
	while (1){
		client.readFromSocket();
	}
}