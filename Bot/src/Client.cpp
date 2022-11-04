#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

Client::Client(int port, std::string host, std::string pass) : _port(port), _host(host), _pass(pass)
{
    std::string str;
    std::ifstream inputFileStream("../bot/dictionary.txt");
    if(!inputFileStream.is_open()) {
        std::cerr << "Can't open the file" << std::endl;
    } else {
        while(getline(inputFileStream, str)) {
            std::string key = str.substr(1, str.find("\"", 1) - 1);
            std::string value = str.substr(str.find_last_of(";") + 2);
            value = value.erase(value.size() - 1);
            dictionary.insert(std::pair<std::string, std::string>(key, value));
        }
    }
    inputFileStream.close();
}

Client::~Client()
{
}


void Client::connectToPort(){
	sockaddr_in address;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		std::cout << "Error! Can't open socket for connections.\n";
		exit(EXIT_FAILURE);
	}

	int yes = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (connect(_socket, (sockaddr*)&address, sizeof(address)) < 0){
		std::cout << "Can't connect to port\n";
		exit(EXIT_FAILURE);
	}
}

void Client::readFromSocket(){
	char buf[512];
	int r = 0;
	memset(buf, '\0', 512);
	r = recv(_socket, buf, 512, 0);
	if (r == 0)
		exit(EXIT_FAILURE);
    std::string str(buf);
    if (str.find("PRIVMSG") != str.npos) {
        std::string translation = parseCommand(buf);
        std::string msg = "PRIVMSG " + nickname + " :" + translation + "\r\n";
        send(_socket, msg.c_str(), msg.size(), 0);
    }
}

void Client::sendPassToServer(){
	std::string message = "";

	message = "PASS " + _pass + "\r\n";
	send(_socket, message.c_str(), message.size(), 0);
	message = "USER bot\r\n";
	send(_socket, message.c_str(), message.size(), 0);
	message = "NICK bot\r\n";
	send(_socket, message.c_str(), message.size(), 0);
}

std::string Client::parseCommand(char *buffer) {
    std::string word(buffer);
    nickname = word.substr(1, word.find_first_of("!") - 1);
    word = word.substr(word.find("PRIVMSG") + INDEX_OF_WORD);
    word.erase(word.end() - 2, word.end());
    std::map<std::string, std::string>::iterator it = dictionary.find(word);
    if (it != dictionary.end()) {
        return dictionary[word];
    }
    return "Word not found";
}