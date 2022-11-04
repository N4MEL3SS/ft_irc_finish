#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include "numeric_replies.hpp"

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif

#define RED "\033[31m"
#define GREEN "\033[3;32m"
#define YELLOW "\033[3;33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define GRAY "\033[90m"
#define RESET "\033[0m"

#define CHANNEL_MODE_SIZE 4
#define CHANNEL_MODE_O_INDEX 0 // Флаг оператора
#define CHANNEL_MODE_K_INDEX 1 // Флаг пароля
#define CHANNEL_MODE_L_INDEX 2 // Флаг количества пользователей
#define CHANNEL_MODE_T_INDEX 3 // Флаг топика

#define CHANNEL_O_FLAG 2
#define CHANNEL_V_FLAG 1
#define CHANNEL_N_FLAG 0

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
void printConfigFileFields(const config_file& config);

std::queue<std::string> split(const std::string &str, char sep);
void replaceCRLF(std::string& msg);

int sendError(int user_fd, int reply, const std::string& arg);
void sendReply(int user_fd, int reply, const std::string& nick = "", const std::string& arg = "",
               const std::string &conf_name = "");
void sendToClient(int fd, std::string& msg);

#endif //UTILS_HPP
