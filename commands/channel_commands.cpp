#include "Server.hpp"

int Server::joinCmd(User& user, Message& msg)
{
	if (checkChannelsError(user, msg))
		return -1;

	std::queue<std::string>	keys = split(msg.getParamsStr(), ' ');
	std::queue<std::string> chans = split(keys.front(), ',');

	keys.pop();
	if (!keys.empty())
		keys = split(keys.front(), ',');

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

		Channel& ref_chan = *_channels_map[chans.front()];

		createAnswerJoin(user, msg, chans.front());
		sendToClient(user.getUserFD(), msg.getAnswerForClient());

		std::string send_msg = ":" + _config.server_name + " 331 " + user.getNickName() + " " + chans.front() + " :No topic is set";
		sendToClient(user.getUserFD(), send_msg);

		send_msg = ":" + _config.server_name + " 353 " +  user.getNickName() + " = " + chans.front() + " :";
		std::map<std::string, int>::iterator it_b = ref_chan.getChannelUsers().begin();
		std::map<std::string, int>::iterator it_e = ref_chan.getChannelUsers().end();

		for (; it_b != it_e; it_b++)
		{
			if (it_b->second == CHANNEL_O_FLAG)
				send_msg += "@";
			send_msg += it_b->first + " ";
		}
		//send_msg.replace(send_msg.size() - 1, 1, "");
		sendToClient(user.getUserFD(), send_msg);

//		sendReply(user.getUserFD(), RPL_ENDOFNAMES, user.getNickName());
		send_msg = ":" + _config.server_name + " 366 " + user.getNickName() + " " + chans.front() + " :End of /NAMES list";
		sendToClient(user.getUserFD(), send_msg);

		chans.pop();
		if (!keys.empty())
			keys.pop();
	}

	return 0;
}

void Server::addUser(User& user, Channel &chan)
{
	chan.getChannelUserNickMap()[user.getNickName()] = &user;

	if (chan.getChannelOperators().empty())
	{
		chan.getChannelOperators().push_back(user.getNickName());
		chan.getChannelUsers()[user.getNickName()] = CHANNEL_O_FLAG;
	}
	else
	{
		chan.getChannelUsers()[user.getNickName()] = CHANNEL_N_FLAG;
	}
}

int Server::checkChannelsError(User& user, Message &msg)
{
	if (msg.getParams().empty() && msg.getPostfix().empty())
		return sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());
	else if (_channels_map.size() >= _config.max_channels)
		return sendError(user.getUserFD(), ERR_TOOMANYCHANNELS, intToString(_channels_map.size()));

	return 0;
}

int Server::partCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		return sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());

	std::queue<std::string>	chans = split(msg.getParams()[0], ',');
	while (!chans.empty())
	{
		if (_channels_map.find(chans.front()) == _channels_map.end())
			return sendError(user.getUserFD(), ERR_NOSUCHCHANNEL, chans.front());
		else if (_channels_map[chans.front()]->getChannelUserNickMap().find(user.getNickName()) == \
			_channels_map[chans.front()]->getChannelUserNickMap().end())
			return sendError(user.getUserFD(), ERR_NOTONCHANNEL, chans.front());
		// TODO: Проверить на нового оператора
		// Удаление данных пользователя с канала
		else
		{
			Channel& ref_chan = *_channels_map[chans.front()];

			std::vector<std::string>::iterator it;
			it = std::find(ref_chan.getChannelOperators().begin(), \
					ref_chan.getChannelOperators().end(), user.getNickName());
			ref_chan.getChannelOperators().erase(it);
			ref_chan.getChannelUsers().erase(ref_chan.getChannelUsers().find(user.getNickName()));
			ref_chan.getChannelUserNickMap().erase(ref_chan.getChannelUserNickMap().find(user.getNickName()));
		}
		chans.pop();
	}


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
