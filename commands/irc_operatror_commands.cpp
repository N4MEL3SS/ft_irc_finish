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
		for (int i = 0; i < _users_pollfd.size(); i++){
			_delete_users.push_back(_users_pollfd[i].fd);
		}
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
		sendReply(user.getUserFD(), RPL_REHASHING, _path_to_config_file);
		printConfigFileFields(_config);
	}
	return 0;
}


int Server::killCmd(User &user, Message &message){
	if (checkIrcOperatorStatus(user)){
		std::vector<std::string> splitted = message.getParams();
		if (splitted.size() < 2){
			replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, "");
			return (-1);
		}
		if (_users_nick_map.find(splitted.front()) == _users_nick_map.end())
		{
			replyError(user.getUserFD(), ERR_NOSUCHNICK, splitted.front());
			return (-1);
		}
		User userToKill = *_users_nick_map[splitted.front()];
		int fd_to_kill = userToKill.getUserFD();
/*		delete _users_nick_map[splitted.front()];
		_users_nick_map.erase(splitted.front());
		_users_fd_map.erase(fd_to_kill);*/
		std::string msg = "";
		_delete_users.push_back(fd_to_kill);
		splitted.erase(splitted.begin());
		while (!splitted.empty()){
			msg += splitted.front() + " ";
			splitted.erase(splitted.begin());
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
	user.setIrcOperatorStatus();
	sendReply(user.getUserFD(), RPL_YOUREOPER);
	return (0);
}



