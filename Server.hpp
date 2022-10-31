#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Utils.hpp"

class Server;

typedef int (Server::*Method)(User&, Message&);

class Server
{
 private:
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


	// Файловый дескриптор админа
	int admin_server_fd;

	// Словарь для хранения ссылок на функции-команды
	std::map<std::string, Method> _commands_map;

	// Словарь пользователей, где ключом служит их fd, а значением объект класса User
	std::map<int, User *> _users_fd_map;
	// Словарь пользователей, где ключом служит их ник ники, а значением объект класса User
	std::map<std::string, User *> _users_nick_map;

	// Словарь каналов, где ключом служит их название, а значением объект класса Channel
	std::map<std::string, Channel *> _channels_map;

	// Вектор для хранения fd пользователей которых нужно удалить
	std::vector<int> _delete_users;

 public:
	~Server();
	Server(int port, const std::string& password, \
			const std::string& path_to_conf = "./conf/conf.conf");

	void initServer();
	void initCommands();
	void acceptConnection();
	void messageProcessing();

	void messageHandler(int user_fd);

	int passCmd(User &user, Message &msg);
	int userCmd(User &user, Message &msg);
	int nickCmd(User &user, Message &msg);
	int quitCmd(User &user, Message &msg);

	int privmsgCmd(User &user, Message &msg);

	int joinCmd(User &user, Message &msg);

	int checkConnection(User& user);
	void sendMOTD(User& user);

	config_file getServerConfig() const;
};

#endif //SERVER_HPP
