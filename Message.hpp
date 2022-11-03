#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Utils.hpp"
#include "User.hpp"

#define BUFF_SIZE 128

class Message
{
 private:
	std::string						_command;
	std::string						_prefix;
	std::vector<std::string>		_postfix;
	std::string						_postfix_str;
	std::vector<std::string>		_parameters;
	std::string						_parameters_str;

	std::string 					_message_raw;
	std::queue<std::string>			_message_queue;

	std::string						_answer_for_client;

 public:
	Message();
	~Message();

	void readMessage(int fd, User& user);
	void parsingMessage();
	void joinString(std::vector<std::string>& dst, std::string& src);

	std::queue<std::string> getMessageQueue();
	void clearData();

	// Getter
	const std::string				&getCommand() const;
	const std::string				&getPrefix() const;
	const std::vector<std::string>	&getPostfix() const;
	const std::string				&getPostfixStr() const;
	const std::vector<std::string>	&getParams() const;
	const std::string				&getParamsStr() const;
	std::string						&getAnswerForClient();

	void setAnswerForClient(const std::string& answer);
};

#endif //MESSAGE_HPP
