#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include <vector>

#include <climits>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define GRAY "\033[90m"
#define RESET "\033[0m"

struct config_file
{
	std::string server_name;
	std::string info;
	std::string admin_name;
	std::string admin_nickname;
	std::string admin_email;
	std::string operator_password;
	int max_channels;

	config_file()
	{
		server_name = "server";
		info = "none";
		admin_name = "";
		admin_nickname = "";
		admin_email = "";
		operator_password = "qwerty";
		max_channels = 10;
	}
};

std::string intToString(int number);

config_file parseConfigFile(const std::string& path);

std::queue<std::string> split(const std::string &str, char sep);

#endif //UTILS_HPP
