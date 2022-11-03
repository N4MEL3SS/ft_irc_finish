#include "Utils.hpp"

void sendReply(int user_fd, int reply, const std::string& nick, const std::string& arg)
{
	std::string message = intToString(reply) + ' ' + nick;

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
	}
	sendToClient(user_fd, message);
}
