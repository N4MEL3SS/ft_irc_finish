#include "Server.hpp"

int Server::isonCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		sendError(user.getUserFD(), ERR_NEEDMOREPARAMS, msg.getCommand());

	std::string nickname_list;

	for (int i = 0; i < (int)msg.getParams().size(); i++)
	{
		if (_users_nick_map.find(msg.getParams()[i]) != _users_nick_map.end())
		{
			if (!nickname_list.empty())
				nickname_list += " ";
			nickname_list += msg.getParams()[i];
		}
	}

	if (nickname_list.empty())
		nickname_list = "";
	sendToClient(user.getUserFD(), nickname_list);

	return 0;
}

int Server::pingCmd(User& user, Message& msg)
{
	if (msg.getParams().empty())
		return sendError(user.getUserFD(), ERR_NOORIGIN, "");

	std::string answer = ":" + _config.server_name + " PONG :" + msg.getParams()[0];

	sendToClient(user.getUserFD(), answer);

	return 0;
}

int Server::quitCmd(User& user, Message& msg)
{
	if (!msg.getCommand().empty())
	{
		user.setConnectionStatus(false);
		std::cout << RED << "Exit from server: " << RESET << user.getNickName() << "\n\n" << std::endl;

		return DISCONNECT;
	}

	return 0;
}
