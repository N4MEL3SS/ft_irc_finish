#include "Server.hpp"

void Server::initCommands()
{
	_commands_map["PASS"] = &Server::passCmd;
	_commands_map["USER"] = &Server::userCmd;
	_commands_map["NICK"] = &Server::nickCmd;
	_commands_map["QUIT"] = &Server::quitCmd;

	_commands_map["PRIVMSG"] = &Server::privmsgCmd;

	_commands_map["PING"] = &Server::pingCmd;
//	_commands_map["PONG"] = &Server::privmsgCmd;

//	_commands_map["JOIN"] = &Server::
//	_commands_map["INVITE"] = &Server::
//	_commands_map["PART"] = &Server::
//	_commands_map["WHO"] = &Server::
//
//	_commands_map["KICK"] = &Server::
//	_commands_map["TOPIC"] = &Server::
//	_commands_map["LIST"] = &Server::
//
//	_commands_map["OPER"] = &Server::
//	_commands_map["KILL"] = &Server::
//	_commands_map["REHASH"] = &Server::
//	_commands_map["RESTART"] = &Server::
//
//	_commands_map["NAMES"] = &Server::
}

int Server::passCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (user.getRegistrationStatus())
		replyError(user.getUserFD(), ERR_ALREADYREGISTRED, "");
	else if (msg.getParams().back() != _server_password)
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
		user.setRealName(msg.getParams().back());
		user.setUserName(msg.getPostfix()[0]);
	}

	return 0;
}

// TODO: Добавить верификацию ника
int Server::nickCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		replyError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (_users_nick_map.find(msg.getParams()[0]) != _users_nick_map.end())
		replyError(user.getUserFD(), ERR_NICKNAMEINUSE, msg.getParams()[0]);
	else
		user.setNickName(msg.getParams()[0]);

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
	else
		return DISCONNECT;

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

int Server::quitCmd(User& user, Message& msg)
{
	if (!msg.getCommand().empty())
	{
		user.setConnectionStatus(false);
		return DISCONNECT;
	}

	return 0;
}

int Server::pingCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		replyError(user.getUserFD(), ERR_NOORIGIN, "");
	else
	{
		std::string answer;
		answer = ":" + _config.server_name + " PONG :" + msg.getParams()[0] + "/r/n";

		sendAnswer(user.getUserFD(), answer);
	}

	return 0;
}

int Server::privmsgCmd(User& user, Message& msg)
{
	return 0;
}
