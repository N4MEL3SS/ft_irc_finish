#include "Server.hpp"
#include "User.hpp"

bool Server::checkIrcOperatorStatus(User &user){
	if (!user.getIrcOperatorStatus()){
		replyError(user.getUserFD(), ERR_NOPRIVILEGES, "");
		return (false);
	}
	return true;
}

int Server::restartCmd(User &user, Message &message){
	(void) message;
	if (checkIrcOperatorStatus(user)){
		deleteUsersFromServer();
		close(_socket);
		initServer();
	}
	return 0;
}

int Server::rehashCmd(User &user, Message &message){
	(void) message;
	if (checkIrcOperatorStatus(user)){
		_config = parseConfigFile(_path_to_config_file);
		std::string message = "382 " + _path_to_config_file + " :Rehashing\r\n";
		send(user.getUserFD(), message.c_str(), message.size(), 0);
		printConfigFileFields(_config);
	}
	return 0;
}


int Server::killCmd(User &user, Message &message){
	if (checkIrcOperatorStatus(user)){
		std::queue<std::string> splitted = split(message.getCommand(), ' ');
		if (splitted.size() < 3){
			replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, "");
			return (-1);
		}
		splitted.pop();
		if (_users_nick_map.find(splitted.front()) == _users_nick_map.end())
		{
			replyError(user.getUserFD(), ERR_NOSUCHNICK, splitted.front());
			return (-1);
		}
		User userToKill = *_users_nick_map[splitted.front()];
		int fd_to_kill = userToKill.getUserFD();
		delete _users_nick_map[splitted.front()];
		_users_nick_map.erase(splitted.front());
		_users_fd_map.erase(fd_to_kill);
		std::string msg = "";
		splitted.pop();
		while (!splitted.empty()){
			msg += splitted.front();
			splitted.pop();
		}
		sendAnswer(fd_to_kill, msg);
		close(fd_to_kill);
	}
	return 0;
}

int Server::operCmd(User &user, Message &message){
	std::string password = message.getParamsStr();
	if (!password.size()){
		replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, "");
		return (-1);
	}
	if (password.compare(_config.operator_password)){
		replyError(user.getUserFD(), ERR_PASSWDMISMATCH, "");
		return (-1);
	}
	std::string msg = "381 :You are now an IRC operator";
    user.setIrcOperatorStatus();
	sendAnswer(user.getUserFD(), msg);
	return (0);
}



