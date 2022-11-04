#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#define INDEX_OF_WORD 13

class Client
{
private:
    int _port;
    std::string _host;
    std::string _pass;
    std::map<std::string, std::string> dictionary;
    std::string nickname;
	int _socket;
public:
	Client(int port, std::string host, std::string password);
    ~Client();
	void connectToPort();
	void readFromSocket();
	void sendPassToServer();
    std::string parseCommand(char buffer[]);
};
