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

	// Структура пользовательских подключений от клиента
	std::vector<struct pollfd> _users_pollfd;

	// Основные настройки сервера
	std::string _server_password;
	sockaddr_in _address;
	int			_socket;
	int			_port;

	// Переменные для хранения пути и конфигурационных полей сервера
	std::string _path_to_config_file;
	config_file _config;

	// Файловый дескриптор админа
	int _admin_server_fd;

	// Словарь для хранения ссылок на функции-команды
	std::map<std::string, Method> _commands_map;

	// Словарь пользователей, где ключом служит их fd, а значением ссылка объект класса User
	std::map<int, User *> _users_fd_map;
	// Словарь пользователей, где ключом служит их ник ники, а значением ссылка объект класса User
	std::map<std::string, User *> _users_nick_map;

	// Словарь каналов, где ключом служит их название, а значением ссылка объект класса Channel
	std::map<std::string, Channel *> _channels_map;

	// Вектор для хранения fd пользователей которых нужно удалить
	std::vector<int> _delete_users;

 public:
	Server(int port, const std::string& password, const std::string& path_to_conf = "./conf/conf.conf");
	~Server();

	void initServer();
	void initCommands();
	void acceptConnection();
	void messageProcessing();

	void messageHandler(int user_fd);

	config_file getServerConfig() const;

	// Список поддерживаемых команд
	// Команды регистрации пользователя на сервере (user_commands.cpp)
	int passCmd(User &user, Message &msg);
	int userCmd(User &user, Message &msg);
	int nickCmd(User &user, Message &msg);
	int quitCmd(User &user, Message &msg);
	// Вспомогательные команды для регистрации
	int checkConnection(User& user);
	void sendMOTD(User& user);

	int privmsgCmd(User &user, Message &msg);
	void createAnswer(User& user, Message& msg, std::string recepient);

	int joinCmd(User &user, Message &msg);
	int part(User& user, Message& msg);
	int modeCmd(User& user, Message& msg);
	int checkChannelsError(User& user, Message& msg);

	int pingCmd(User& user, Message& msg);
	int isonCmd(User& user, Message& msg);
	int whoCmd(User& user, Message& chan);

	int findPollfd(int fd);

	void deleteUsersFromServer();
	void deleteEmptyChannels();

	void addUser(User& user, Channel& chan);

	/*IrcOperatorCommands*/
	int operCmd(User &user, Message &message);
	int restartCmd(User &user, Message &message);
	int killCmd(User &user, Message &message);
	int rehashCmd(User &user, Message &message);

	bool checkIrcOperatorStatus(User &user);

	void sendPrivmsgChannel(User& user, Message& msg, const std::string& recipient);


};

#endif //SERVER_HPP
