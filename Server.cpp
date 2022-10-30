#include "Server.hpp"

Server::Server(int port, const std::string& password, const std::string& path_to_conf) :
		_server_password(password), _port(port), _path_to_config_file(path_to_conf)
{
	_config = parseConfigFile(path_to_conf);
}

Server::~Server()
{
	for (int i = 0; i < static_cast<int>(_users_fd_map.size()); i++)
	{
		// TODO: Необходимо ли удалять запись из словаря??? Проверить на утечки
		delete _users_fd_map[_users_pollfd[i].fd];
	}
}

void Server::initServer()
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		std::perror("Error! Can't open socket for connections");
		exit(EXIT_FAILURE);
	}

	int yes = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);

	if (bind(_socket, (sockaddr*)&_address, sizeof(_address)) < 0)
	{
		std::perror("Error! Can't bind socket");
		exit(EXIT_FAILURE);
	}

	if (listen(_socket, FD_SETSIZE) < 0)
	{
		std::perror("Error! Listen failed");
		exit(EXIT_FAILURE);
	}

	fcntl(_socket, F_SETFL, O_NONBLOCK);
}

void Server::acceptConnection()
{
	size_t address_size = sizeof(_address);
	// TODO: Зачем я это добавил???
//	_current_connections = poll(_users_pollfd.data(), _users_pollfd.size(), 0);

	int new_connection = accept(_socket, (sockaddr*)&_address, (socklen_t *)&address_size);
	if (new_connection > 0)
	{
		std::cout << GREEN << "New user connected" << RESET << "\n";

		struct pollfd pfd = {};
		pfd.fd = new_connection;
		pfd.events = POLLIN;
		pfd.revents = 0;

		_users_pollfd.push_back(pfd);
		_users_fd_map[pfd.fd] = new User(pfd.fd);
	}
}

void Server::messageProcessing()
{
	//TODO: добавить timeout?
	_current_connections = poll(_users_pollfd.data(), _users_pollfd.size(), 0);

	if (_current_connections > 0)
	{
		int fd_count = static_cast<int>(_users_pollfd.size());
		for (int i = 0; i < fd_count; i++)
		{
//			if (_users_pollfd[i].revents & POLLIN && _users_fd_map[_users_pollfd[i].fd]->getConnectionStatus())
			if (_users_pollfd[i].revents & POLLIN)
			{
				messageHandler(_users_pollfd[i].fd);
			}
			_users_pollfd[i].revents = 0;
		}
	}
}

void Server::messageHandler(int user_fd)
{
	clientMessage.readMessage(user_fd, *_users_fd_map[user_fd]);

	while (!clientMessage.getMessageQueue().empty())
	{
		clientMessage.parsingMessage();

		if (!_users_fd_map[user_fd]->getRegistrationStatus())
		{
			registrationUser(user_fd);
		}

		if (commands.getCommandsMap().find(clientMessage.getCommand()) != commands.getCommandsMap().end())
		{
			commands.getCommandsMap().at(clientMessage.getCommand());
		}
		else
		{
			ErrorReply(user_fd, ERR_UNKNOWNCOMMAND, clientMessage.getCommand());
		}
	}
}

void Server::registrationUser(int user_fd)
{
	std::string command = clientMessage.getCommand();
	_users_fd_map[user_fd]->incRegistrationStage();

	// TODO: Переделать через ссылку на registration_stage и arr_reg_command
	if (_users_fd_map[user_fd]->getRegistrationStage() == 1 and command != "PASS")
	{
		_users_fd_map[user_fd]->zeroRegistrationStage();
		ErrorReply(user_fd, ERR_NOTREGISTERED, "");
	}
	else if (_users_fd_map[user_fd]->getRegistrationStage() == 2 and command != "USER")
	{
		_users_fd_map[user_fd]->zeroRegistrationStage();
		ErrorReply(user_fd, ERR_NOTREGISTERED, "");
	}
	else if (_users_fd_map[user_fd]->getRegistrationStage() == 3 and command != "NICK")
	{
		_users_fd_map[user_fd]->zeroRegistrationStage();
		ErrorReply(user_fd, ERR_NOTREGISTERED, "");
	}
	if (_users_fd_map[user_fd]->getRegistrationStage() == 4)
}

void Server::ErrorReply(int user_fd, int reply, std::string arg)
{
	std::string message = intToString(reply) + ' ' + arg;

	switch(reply)
	{
	case ERR_NOTREGISTERED:
		message += " :You have not registered";
		break;
	case ERR_PASSWDMISMATCH:
		message += " :Password incorrect";
		break;
	case ERR_NEEDMOREPARAMS:
		message += " :Not enough parameters";
		break;
	case ERR_NICKNAMEINUSE:
		message += " :Nickname is already in use";
		break;
	case ERR_NOSUCHNICK:
		message += " :No such nick/channel";
		break ;
	case ERR_NOSUCHCHANNEL:
		message += " :No such channel";
		break;
	case ERR_NONICKNAMEGIVEN:
		message += " :No nickname given";
		break;
	case ERR_ERRONEUSNICKNAME:
		message += " :Erroneus nickname";
		break;
	case ERR_NOTONCHANNEL:
		message += " :They aren't on that channel";
		break;
	case ERR_ALREADYREGISTRED:
		message += " :You may not reregister";
		break;
	case ERR_NOPRIVILEGES:
		message += " :Permission Denied- You're not an IRC operator";
		break;
	case ERR_NORECIPIENT:
		message += " :No recipient given";
		break;
	case ERR_UNKNOWNCOMMAND:
		message += " :Unknown command";
		break;
	}
	message += "\r\n";
	send(user_fd, message.c_str(), message.size(), 0);
}

void Server::printConfigFileFields() const
{
	std::cout << BLUE;
	std::cout << "__________________ ===Server config=== __________________\n";
	std::cout << "server name: " << _config.server_name << '\n';
	std::cout << "info: " << _config.info << '\n';
	std::cout << "admin name: "  << _config.admin_name << '\n';
	std::cout << "admin nickname: " << _config.admin_nickname << '\n';
	std::cout << "admin email: " << _config.admin_email << '\n';
	std::cout << "Operator password doesn't show up due to security reasons\n";
	std::cout << "maximum channels on server: " << _config.max_channels << '\n';
	std::cout << "---------------------------------------------------------\n";
	std::cout << RESET << std::endl;
}
