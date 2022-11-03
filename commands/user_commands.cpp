#include "Server.hpp"

void Server::sendPrivmsgChannel(User& user, Message& msg, const std::string& recipient)
{
	std::map<std::string, User*>::iterator it_b_users;
	std::map<std::string, User*>::iterator it_e_users;

	// TODO: Придумать нормальное имя
	std::map<std::string, User*> map = _channels_map[recipient]->getChannelUserNickMap();
	it_b_users = map.find(user.getNickName());
	it_e_users = map.end();

	// TODO: Ошибка для каналов с модом +n
	if (it_b_users == it_e_users)
		sendError(user.getUserFD(), ERR_CANNOTSENDTOCHAN, recipient);
	else
	{
		it_b_users = map.begin();
		for (; it_b_users != it_e_users; it_b_users++)
		{
			createAnswer(*it_b_users->second, msg, recipient);
			sendToClient(it_b_users->second->getUserFD(), msg.getAnswerForServer());
		}
	}
}

//void Server::sendPrivmsgUser()
//{
//
//}

int Server::privmsgCmd(User& user, Message& msg)
{
	if (msg.getPostfix().empty())
		return sendError(user.getUserFD(), ERR_NORECIPIENT, msg.getCommand());
	else if (msg.getParams().empty())
		return sendError(user.getUserFD(), ERR_NOTEXTTOSEND, msg.getCommand());

	std::queue<std::string> recipients = split(msg.getPostfix().front(), ',');
	std::string to_client;

	while (!recipients.empty())
	{
		if ((recipients.front()[0] == '#' || recipients.front()[0] == '&') && \
			_channels_map.find(recipients.front()) != _channels_map.end())
		{

		}
		else if (_users_nick_map.find(recipients.front()) != _users_nick_map.end())
		{
//			to_client = createAnswer(user, msg, recipients.front());
			sendToClient(_users_nick_map[recipients.front()]->getUserFD(), msg.getAnswerForServer());
		}
		else
			sendError(user.getUserFD(), ERR_NOSUCHNICK, recipients.front());

		recipients.pop();
	}

	return 0;
}

void Server::createAnswer(User& user, Message& msg, std::string recepient)
{
	msg.setAnswerForServer(":" + user.getFullName() + " " + msg.getCommand() + " " + recepient + " :" + msg.getParamsStr());
}
