#include "Server.hpp"

void Server::initCommands()
{
	_commands_map["PASS"] = &Server::passCmd;
	_commands_map["USER"] = &Server::userCmd;
	_commands_map["NICK"] = &Server::nickCmd;
	_commands_map["QUIT"] = &Server::quitCmd;

	_commands_map["ISON"] = &Server::isonCmd;

	_commands_map["PRIVMSG"] = &Server::privmsgCmd;
	_commands_map["NOTICE"] = &Server::privmsgCmd;

	_commands_map["PING"] = &Server::pingCmd;
//	_commands_map["PONG"] = &Server::privmsgCmd;

	_commands_map["JOIN"] = &Server::joinCmd;
	_commands_map["MODE"] = &Server::modeCmd;
//	_commands_map["PART"] = &Server::
	_commands_map["WHO"] = &Server::whoCmd;
//
//	_commands_map["INVITE"] = &Server::
//	_commands_map["KICK"] = &Server::
//	_commands_map["TOPIC"] = &Server::
//	_commands_map["LIST"] = &Server::
//
	_commands_map["OPER"] = &Server::operCmd;
	_commands_map["KILL"] = &Server::killCmd;
	_commands_map["REHASH"] = &Server::rehashCmd;
	_commands_map["RESTART"] = &Server::restartCmd;
//
//	_commands_map["NAMES"] = &Server::
}
