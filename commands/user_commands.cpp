#include "Server.hpp"

int Server::privmsgCmd(User& user, Message& msg)
{
	if (msg.getPostfix().empty())
	{
		replyError(user.getUserFD(), ERR_NORECIPIENT, msg.getCommand());
		return 0;
	}
	else if (msg.getParams().empty())
	{
		replyError(user.getUserFD(), ERR_NOTEXTTOSEND, msg.getCommand());
		return 0;
	}

	for (int i = 0; i < msg.getPostfix().size(); i++)
	{
		std::string to_client;
		if (msg.getPostfix()[i][0] == '#' || msg.getPostfix()[i][0] == '&')
		{
			to_client = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost " + \
				msg.getCommand() + " " + msg.getPostfix()[i] + " :" + msg.getParamsStr();
			sendAnswer(_users_fd_map[user.getUserFD()]->getUserFD(), to_client);
		}
		else if (_users_nick_map.find(msg.getPostfix()[i]) != _users_nick_map.end())
		{
			// TODO: Вынести в отдельную функцию
			to_client = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost " + \
				msg.getCommand() + " " + msg.getPostfix()[i] + " :" + msg.getParamsStr();
			// TODO: добавить цикл
			sendAnswer(_users_nick_map[msg.getPostfix()[i]]->getUserFD(), to_client);
		}
		else
			replyError(user.getUserFD(), ERR_NOSUCHNICK, msg.getPostfix()[i]);
	}

	return 0;
}

std::string Server::createAnswerString(User& user, Message& msg)
{
	std::string to_client;

	to_client = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost " + msg.getCommand() + " " + msg.getParamsStr();

	return to_client;
}
