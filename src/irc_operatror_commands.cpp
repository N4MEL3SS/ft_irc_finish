#include "Server.hpp"
#include "User.hpp"

bool Server::checkIrcOperatorStatus(User &user)
{
	if (!user.getIrcOperatorStatus())
	{
		sendError(user.getUserFD(), ERR_NOPRIVILEGES, "");
		return (false);
	}

	return true;
}

int Server::restartCmd(User &user, Message &message){
	(void) message;

	if (checkIrcOperatorStatus(user))
	{
		for (int i = 0; i < (int)_users_pollfd.size(); i++)
			_delete_users.push_back(_users_pollfd[i].fd);

		deleteUsersFromServer();
		close(_socket);
		initServer();
	}

	return 0;
}

int Server::rehashCmd(User &user, Message &message){
	(void) message;

	if (checkIrcOperatorStatus(user))
	{
		_config = parseConfigFile(_path_to_config_file);
		sendReply(user.getUserFD(), RPL_REHASHING, _path_to_config_file);
		printConfigFileFields(_config);
	}

	return 0;
}


int Server::killCmd(User &user, Message &message){
	if (checkIrcOperatorStatus(user))
	{
		std::vector<std::string> splitted = message.getParams();

		if (splitted.size() < 2)
			return sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, "");

		if (_users_nick_map.find(splitted.front()) == _users_nick_map.end())
			return sendError(user.getUserFD(), ERR_NOSUCHNICK, splitted.front());

		int fd_to_kill = _users_nick_map[splitted.front()]->getUserFD();
		std::string msg = message.getParamsStr().substr(splitted.front().size() + 1, std::string::npos);

		_delete_users.push_back(fd_to_kill);
		sendToClient(fd_to_kill, msg);

		// TODO: в deleteUsersFromServer уже выполняется close(). Нужно ли здесь?
		close(fd_to_kill);
	}

	return 0;
}

int Server::operCmd(User &user, Message &message){
	const std::string& password = message.getParamsStr();

	if (password.empty())
		return sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, "");

	if (password.compare( _config.operator_password))
		return sendError(user.getUserFD(), ERR_PASSWDMISMATCH, "");

	user.setIrcOperatorStatus(true);
	sendReply(user.getUserFD(), RPL_YOUREOPER);

	return 0;
}
