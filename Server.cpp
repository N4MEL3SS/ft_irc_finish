#include "Server.hpp"

Server::Server(int port, const std::string& password, const std::string& path_to_conf) :
		_server_password(password), _port(port), _path_to_config_file(path_to_conf)
{
	_config = parseConfigFile(path_to_conf);
//	printConfigFileFields(_config);
}

Server::~Server()
{
	for (int i = 0; i < static_cast<int>(_users_fd_map.size()); i++)
	{
		// TODO: Необходимо ли удалять запись из словаря??? Проверить на утечки
		close(_users_pollfd[i].fd);
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

	int new_connection = accept(_socket, (sockaddr*)&_address, (socklen_t *)&address_size);
	if (new_connection > 0)
	{
		struct pollfd pfd = {};
		pfd.fd = new_connection;
		pfd.events = POLLIN;
		pfd.revents = 0;

		_users_pollfd.push_back(pfd);
		_users_fd_map[pfd.fd] = new User(pfd.fd);

		std::cout << GREEN << "New user connected fd: " << RESET << pfd.fd << '\n' << std::endl;
	}
}

void Server::messageProcessing()
{
	int connections = poll(_users_pollfd.data(), _users_pollfd.size(), 0);

	if (connections > 0)
	{
		int fd_count = static_cast<int>(_users_pollfd.size());
		for (int i = 0; i < fd_count; i++)
		{
			if (_users_pollfd[i].revents & POLLIN && _users_fd_map[_users_pollfd[i].fd]->getConnectionStatus())
				messageHandler(_users_pollfd[i].fd);
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
		std::string cmd = clientMessage.getCommand();

		if (!_users_fd_map[user_fd]->getRegistrationStatus() && cmd != "PASS" && cmd != "USER" \
			&& cmd != "NICK" && cmd != "QUIT")
		{
			replyError(user_fd, ERR_NOTREGISTERED, "");
		}
		else if (_commands_map.find(cmd) != _commands_map.end())
		{
			int ret = (this->*(_commands_map.at(cmd)))(*_users_fd_map[user_fd], clientMessage);
			if (ret == DISCONNECT)
				_delete_users.push_back(user_fd);
		}
		else
			replyError(user_fd, ERR_UNKNOWNCOMMAND, cmd);
	}
}

void Server::deleteUsersFromServer()
{
	if (!_delete_users.empty())
	{
		for (int i = 0; i < _delete_users.size(); i++)
		{
			std::string nick = _users_fd_map[_delete_users[i]]->getNickName();

			close(_delete_users[i]);

			if (_users_nick_map.find(nick) != _users_nick_map.end())
				_users_nick_map.erase(_users_nick_map.find(nick));

			_users_pollfd.erase(_users_pollfd.begin() + findPollfd(_delete_users[i]));

			delete _users_fd_map[_delete_users[i]];
			_users_fd_map.erase(_users_fd_map.find(_delete_users[i]));
		}
		_delete_users.clear();
	}
}

int Server::findPollfd(int fd)
{
	int i = 0;

	while (i < _users_pollfd.size() && _users_pollfd[i].fd != fd)
		i++;

	return i;
}

config_file Server::getServerConfig() const { return _config; }
