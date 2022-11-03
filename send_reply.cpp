#include "Utils.hpp"

void sendReply(int user_fd, int reply, const std::string& nick, const std::string& arg, const std::string& conf_name)
{
	std::string message = conf_name + intToString(reply) + ' ' + nick;

	switch(reply)
	{
	case RPL_MOTDSTART:
		message += ":- " + arg + " Message of the day -";
		break ;
	case RPL_MOTD:
		message += ":- " + arg;
		break ;
	case RPL_ENDOFMOTD:
		message += ":End of /MOTD command";
		break ;
	case RPL_YOUREOPER:
		message += ":You are now an IRC operator";
		break ;
	case RPL_REHASHING:
		message += arg + ":Rehashing";
		break;
	case RPL_AWAY:
//		message +=
		break;
	case RPL_UNAWAY:
//		message +=
		break;
	case RPL_ISON:
//		message +=
		break;
	case RPL_ENDOFWHO:
		message += nick + " :End of /WHO list";
		break;
	case RPL_NOTOPIC:
//		message +=
		break;
	case RPL_TOPIC:
		message += nick + " :No topic is set";
		break;
	case RPL_NAMREPLY:
//		message +=
		break;
	case RPL_ENDOFNAMES:
		message += nick + " :End of /NAMES list";
		break;
	}

	sendToClient(user_fd, message);
}
