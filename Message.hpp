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
	std::string 					_join_msg;

 public:
	Message();
	~Message();

	void readMessage(int fd, User& user);
	void parsingMessage();
	void joinString(std::vector<std::string>& msg);
//	std::string join_string(std::vector<std::string> msg);

	std::queue<std::string> getMessageQueue();
	void clearData();

	// Getter
	const std::string				&getCommand() const;
	const std::vector<std::string>	&getPrefix() const;
	const std::vector<std::string>	&getPostfix() const;
	const std::vector<std::string>	&getParams() const;
};

#endif //MESSAGE_HPP
