#include "Server.hpp"

int Server::joinCmd(User& user, Message& msg)
{
	if (checkChannelsError(user, msg))
		return 0;

	std::queue<std::string>	keys = split(msg.getParamsStr(), ' ');
	std::queue<std::string> chans = split(keys.front(), ',');
	keys.pop();

	while (!chans.empty())
	{
		if (_channels_map.find(chans.front()) == _channels_map.end())
		{
			if (keys.size() > 1)
				_channels_map[chans.front()] = new Channel(chans.front(), msg.getParams()[1]);
			else
				_channels_map[chans.front()] = new Channel(chans.front(), "");
			addUser(user, *_channels_map[chans.front()]);
		}
		else
		{
			if (keys.empty())
				addUser(user, *_channels_map[chans.front()]);
			else if (keys.front() == _channels_map[chans.front()]->getChannelPassword())
				addUser(user, *_channels_map[chans.front()]);
			else
				sendError(user.getUserFD(), ERR_PASSWDMISMATCH, "");
		}


		createAnswer(user, msg, chans.front());
//		std::string check =
		sendToClient(user.getUserFD(), msg.getAnswerForServer());

//		std::string y = ":" + _config.server_name + " 331 wabathur #asdfg :No topic is set";
//		std::string y = "331 #asdfg :topic";
//		sendToClient(user.getUserFD(), y);

		std::string t = ":" + _config.server_name + " 353 wabathur = #asdfg :@wabathur";
		sendToClient(user.getUserFD(), t);
		t = ":" + _config.server_name + " 366 wabathur #asdfg :End of /NAMES list";
		sendToClient(user.getUserFD(), t);

		chans.pop();
		if (!keys.empty())
			keys.pop();
	}

	return 0;
}

void Server::addUser(User& user, Channel &chan)
{
	chan.getChannelUserNickMap()[user.getNickName()] = &user;
	chan.getChannelUsers().push_back(user.getNickName());

	if (chan.getChannelOperators().empty())
		chan.getChannelOperators().push_back(user.getNickName());

}

int Server::checkChannelsError(User& user, Message &msg)
{
	if (msg.getParams().empty() && msg.getPostfix().empty())
		return sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (_channels_map.size() >= _config.max_channels)
		return sendError(user.getUserFD(), ERR_TOOMANYCHANNELS, intToString(_channels_map.size()));

	return 0;
}

//:ircserv 353 levensta1 #ratata :levensta1!levensta1@localhost
//:ircserv 353 levensta1 #ratata :levensta2!levensta2@localhost
//:ircserv 366 levensta1 #ratata :End of /NAMES list

int Server::whoCmd(User& user, Message &msg)
{
//	std::string g = ":IRCat 315 wabathur wabathur :End of /WHO list";
//	sendToClient(user.getUserFD(), g);

	return 0;
}

int Server::modeCmd(User& user, Message &msg)
{
	// TODO: Проверка на канал
	if (msg.getParams().size() > 1 && _channels_map.find(msg.getParams()[0]) != _channels_map.end())
	{
		Channel& chan = *_channels_map[msg.getParams()[0]];
		if (msg.getParams()[1][0] == '+')
		{
			for (int i = 1; i < msg.getParams().size(); i++)
			{
				if (msg.getParams()[1][i] == 'o')
				{
//					user.getUserChannelMode()[]
				}
			}
		}
	}

	return 0;
}
