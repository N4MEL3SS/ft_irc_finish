#include "Commands.hpp"

Commands::Commands()
{
	_commands_map["PASS"] = &Commands::passCmd;
	_commands_map["NICK"] = &Commands::nickCmd;
	_commands_map["USER"] = &Commands::userCmd;
	_commands_map["QUIT"] = &Commands::quitCmd;
	_commands_map["PRIVMSG"] = &Commands::privmsgCmd;
}

Commands::~Commands() {}

void Commands::passCmd(User& user, Message& msg)
{

}

void Commands::nickCmd(User& user, Message& msg)
{

}

void Commands::userCmd(User& user, Message& msg)
{

}

void Commands::privmsgCmd(User& user, Message& msg)
{

}

void Commands::quitCmd(User& user, Message& msg)
{

}

std::map<std::string, Method> Commands::getCommandsMap() const { return this->_commands_map; }