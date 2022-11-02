#include "Server.hpp"

int Server::passCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (user.getRegistrationStatus())
		replyError(user.getUserFD(), ERR_ALREADYREGISTRED, "");
	else if (msg.getParamsStr() != _server_password)
		return DISCONNECT;

	return 0;
}

int Server::userCmd(User& user, Message& msg)
{
	if (msg.getParams().empty() && msg.getPostfix().empty())
		replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (user.getRegistrationStatus())
		replyError(user.getUserFD(), ERR_ALREADYREGISTRED, "");
	else
	{
		user.setRealName(msg.getParamsStr());
        if (msg.getPostfix().empty())
		    user.setUserName(msg.getParamsStr());
        else
		    user.setUserName(msg.getPostfix()[0]);
	}

	return checkConnection(user);
}

// TODO: Добавить верификацию ника
// TODO: Считать ли вариант NICK user test валидным?
int Server::nickCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		replyError(user.getUserFD(), ERR_NONICKNAMEGIVEN, "");
	else if (_users_nick_map.find(msg.getParamsStr()) != _users_nick_map.end())
		replyError(user.getUserFD(), ERR_NICKNAMEINUSE, msg.getParamsStr());
	else
		user.setNickName(msg.getParamsStr());

	return checkConnection(user);
}

int Server::checkConnection(User& user)
{
	if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getRegistrationStatus())
	{
		user.setRegistrationStatus(true);
		_users_nick_map[user.getNickName()] = &user;
		sendMOTD(user);
	}
//	else
//		return DISCONNECT;

	return 0;
}

void Server::sendMOTD(User& user)
{
	std::string message;

	message = ":server 375 " + user.getNickName() + " :- Message of the day -\r\n";
	message += ":server 372 " + user.getNickName() + " :- welcome to server\r\n";
	message += ":server 376 " + user.getNickName() + " :End of /MOTD command\r\n";
	message += "001 "+ user.getNickName() + " :Welcome to IRChat, " + user.getNickName() + "!" + user.getUserName() + "@127.0.0.1";

	sendAnswer(user.getUserFD(), message);
}
