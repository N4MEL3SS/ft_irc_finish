#ifndef SERVER_HPP
#define SERVER_HPP

#include "Commands.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Utils.hpp"
#include "numeric_replies.hpp"

class Server
{
 private:
	// Класс Commands хранит в себе ссылки на выполняемые команды. Упрощает вызов команд
	class Commands commands;
	// Класс ClientMessage считывает и парсит входные данные со стороны клинта
	class Message clientMessage;

	std::vector<struct pollfd> _users_pollfd;
	// Основные настройки сервера
	std::string _server_password;
	sockaddr_in _address;
	int			_socket;
	int			_port;
	int			_current_connections;

	std::string _path_to_config_file;
	config_file _config;

	// TODO: Возможно лишнее
	int users_connect_count;

	// Файловый дескриптор админа
	int admin_server_fd;

	// Словари позволяют захешировать данные для быстрого доступа к ним
	// Словарь пользователей, где ключом служит их ник ники, а значением объект класса User
	std::map<std::string, User *> _users_nick_map;
	// Словарь пользователей, где ключом служит их fd, а значением объект класса User
	std::map<int, User *> _users_fd_map;
	// Словарь каналов, где ключом служит их ник название, а значением объект класса Channel
	std::map<std::string, Channel *> channels_map;

 public:
	~Server();
	Server(int port, const std::string& password, \
			const std::string& path_to_conf = "./conf/conf.conf");

	void initServer();
	void acceptConnection();
	void messageProcessing();

	void messageHandler(int user_fd);

	void printConfigFileFields() const;

	void registrationUser();

	void registrationUser(int user_fd);

	void ErrorReply(int user_fd, int reply, std::string arg);
};

#endif //SERVER_HPP
