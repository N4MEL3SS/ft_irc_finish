#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#define INDEX_OF_WORD 13

class Client
{
private:
    std::map<std::string, std::string> dictionary;
    std::string _pass;
    std::string _host;
    std::string nickname;
	int _port;
	int _socket;
public:
	Client(int port, std::string host, std::string password);
    ~Client();
	void connectToPort();
	void readFromSocket();
	void sendPassToServer();
    std::string parseCommand(char buffer[]);
};
