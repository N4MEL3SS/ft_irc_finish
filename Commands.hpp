#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Channel.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "Utils.hpp"

class Commands;

typedef void (Commands::*Method)(User&, Message&);
//typedef void (Commands::*Method)(Channel&, Message&);

class Commands
{
 private:
	std::map<std::string, Method> _commands_map;
 public:
	Commands();
	~Commands();

	// Функции ссылки. Служат для вызова полноценных функций.
	void passCmd(User &user, Message &msg);
	void userCmd(User &user, Message &msg);
	void nickCmd(User &user, Message &msg);

	void privmsgCmd(User &user, Message &msg);

	void joinCmd(User &user, Message &msg);

	void quitCmd(User &user, Message &msg);

	std::map<std::string, Method> getCommandsMap() const;
};

#endif //COMMANDS_HPP
