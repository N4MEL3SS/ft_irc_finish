#include "Utils.hpp"

int sendError(int user_fd, int reply, const std::string& arg)
{
	std::string message = intToString(reply) + ' ' + arg;

	switch(reply)
	{
	case ERR_NOTREGISTERED:
		message += " :You have not registered";
		break;
	case ERR_PASSWDMISMATCH:
		message += " :Password incorrect";
		break;
	case ERR_NEEDMOREPARAMS:
		message += " :Not enough parameters";
		break;
	case ERR_NICKNAMEINUSE:
		message += " :Nickname is already in use";
		break;
	case ERR_NOSUCHNICK:
		message += " :No such nick/channel";
		break ;
	case ERR_NOSUCHCHANNEL:
		message += " :No such channel";
		break;
	case ERR_CANNOTSENDTOCHAN:
		message += " :Cannot send to channel";
		break;
	case ERR_NONICKNAMEGIVEN:
		message += " :No nickname given";
		break;
	case ERR_ERRONEUSNICKNAME:
		message += " :Erroneus nickname";
		break;
	case ERR_NOTONCHANNEL:
		message += " :They aren't on that channel";
		break;
	case ERR_ALREADYREGISTRED:
		message += " :You may not reregister";
		break;
	case ERR_NOPRIVILEGES:
		message += " :Permission Denied- You're not an IRC operator";
		break;
	case ERR_NORECIPIENT:
		message += " :No recipient given";
		break;
	case ERR_UNKNOWNCOMMAND:
		message += " :Unknown command";
		break;
	case ERR_NOORIGIN:
		message += " :No origin specified";
	case ERR_TOOMANYCHANNELS:
		message += " " + arg + " :You have joined too many channels";
	default:
		break;
	}

	sendToClient(user_fd, message);
	return 1;
}
