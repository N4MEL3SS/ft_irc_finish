#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Utils.hpp"
#include "User.hpp"

#define BUFF_SIZE 128

class Message
{
 private:
	std::string						_command;
	std::vector<std::string>		_prefix;
	std::vector<std::string>		_postfix;
	std::vector<std::string>		_parameters;

	std::string 					_message_raw;
	std::queue<std::string>			_message_queue;

 public:
	Message();
//	Message(const std::string &str);

	~Message();

	void readMessage(int fd, User& user);

	// Getter
	const std::string				&getCommand() const;
	const std::vector<std::string>	&getPrefix() const;
	const std::vector<std::string>	&getParams() const;

	void parsingMessage();

	std::queue<std::string> getMessageQueue();

	void clearData();
};

#endif //MESSAGE_HPP
