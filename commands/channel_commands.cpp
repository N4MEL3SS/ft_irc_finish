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

		std::map<std::string, User *>::iterator it_u = ref_chan.getChannelUserNickMap().begin();
		std::map<std::string, User *>::iterator it_u_e = ref_chan.getChannelUserNickMap().end();

		std::map<std::string, int>::iterator it_b = ref_chan.getChannelUsers().begin();
		std::map<std::string, int>::iterator it_e = ref_chan.getChannelUsers().end();
		std::string users_names = "";
		for (; it_b != it_e; it_b++)
		{
			if (it_b->second == CHANNEL_O_FLAG)
				users_names += "@";
			users_names += it_b->first + " ";
		}
		users_names.replace(users_names.size() - 1, 1, "");
		_user_online = users_names;

		for (;it_u != it_u_e; it_u++)
		{
			send_msg = ":" + _config.server_name + " 353 " +  it_u->second->getNickName() + " = " + chans.front() + " :" + users_names;

			sendToClient(it_u->second->getUserFD(), send_msg);

			send_msg = ":" + _config.server_name + " 366 " + it_u->second->getNickName() + " " + chans.front() + " :End of /NAMES list.";
			sendToClient(it_u->second->getUserFD(), send_msg);
		}

		chans.pop();
		if (!keys.empty())
			keys.pop();
	}

	return 0;
}

void Server::addUser(User& user, Channel &chan)
{
	chan.getChannelUserNickMap()[user.getNickName()] = &user;
	chan.setCountUsersPlus();

	if (chan.getChannelOperators().empty())
	{
		chan.getChannelOperators()[user.getNickName()] = &user;
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

			std::map<std::string, User *>::iterator it;
			if ((it = ref_chan.getChannelOperators().find(user.getNickName())) != ref_chan.getChannelOperators().end())
				ref_chan.getChannelOperators().erase(it);

			if (ref_chan.getChannelOperators().empty() && !ref_chan.getChannelUsers().empty())
			{
				std::map<std::string, User *>::iterator it_b;
				it_b = ref_chan.getChannelUserNickMap().begin();
				ref_chan.getChannelOperators()[it_b->first] = it_b->second;
				ref_chan.getChannelUsers()[it_b->first] = CHANNEL_O_FLAG;
			}

			ref_chan.getChannelUsers().erase(ref_chan.getChannelUsers().find(user.getNickName()));
			ref_chan.getChannelUserNickMap().erase(ref_chan.getChannelUserNickMap().find(user.getNickName()));
			ref_chan.setCountUsersMinus();
		}
		chans.pop();
	}

	return 0;
}

int Server::whoCmd(User& user, Message &msg)
{
	std::string g = ":" + _config.server_name + " 352 " + user.getNickName() + " " +  msg.getParamsStr() + " " + user.getUserName()
			+ " * localhost " + user.getNickName() + " H " + ":" + user.getRealName();

	std::map<std::string, User *>::iterator it_b = _channels_map[msg.getParamsStr()]->getChannelUserNickMap().begin();
	std::map<std::string, User *>::iterator it_e = _channels_map[msg.getParamsStr()]->getChannelUserNickMap().end();
	std::string users_names = "";
	for (; it_b != it_e; it_b++){
		g = ":" + _config.server_name + " 352 " + user.getNickName() + " " + msg.getParamsStr() + " " + it_b->second->getUserName()
				+ " * localhost " + it_b->second->getNickName() + " H : " + it_b->second->getRealName();
		sendToClient(user.getUserFD(), g);
	}
	g = ":" + _config.server_name + " 315 " + user.getNickName() + " " + msg.getParamsStr() + " :End of /WHO list.";
	sendToClient(user.getUserFD(), g);
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
