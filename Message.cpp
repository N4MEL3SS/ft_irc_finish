#include "Message.hpp"

Message::Message() {}
Message::~Message() {}

void Message::readMessage(int fd, User& user)
{
	_message_raw.clear();
	if (user.getIsPartialMessage())
	{
		_message_raw += user.getPartialMessage();
		user.getPartialMessage().clear();
		user.setIsPartialMessage(false);
	}

	size_t bytesRead;
	char buff[BUFF_SIZE];

	memset(buff, '\0', BUFF_SIZE);
	while ((bytesRead = recv(fd, buff, BUFF_SIZE - 1, 0)) > 0)
	{
		buff[bytesRead] = '\0';
		_message_raw += buff;
		if (_message_raw.find('\n') != std::string::npos)
			break;
		buff[0] = '\0';
	}

	if (bytesRead <= 0 && _message_raw.empty())
		_message_raw = "QUIT";

	if (_message_raw.length() > 512)
		_message_raw.substr(0, 510) + '\n';

	replaceCRLF(_message_raw);

	size_t cnlr;
	if ((cnlr = _message_raw.find(EOF, 0)) != std::string::npos)
	{
		user.setPartialMessage(_message_raw.substr(0, cnlr));
		user.setIsPartialMessage(true);
		_message_raw.clear();
	}

	_message_queue = split(_message_raw, '\n');

	if (std::count(_message_raw.begin(), _message_raw.end(), '\n') > 1)
		std::cout << YELLOW << "Input from client [" << fd << "]\n" << RESET;
	else
		std::cout << YELLOW << "Input from client [" << fd << "]: " << RESET;
	std::cout << CYAN << _message_raw << RESET << std::endl;
}

void Message::parsingMessage()
{

	std::queue<std::string> split_msg = split(_message_queue.front(), ' ');
	bool postfix_flag = false;

	// Очищает переменные _prefix, _postfix, _parameters от данных с предыдущего запуска
	clearData();

	// Флаг для заполнения _postfix.
	// Помещает данные между командой и параметрами в _postfix если они имеются
	if (split_msg.front()[0] != ':' and _message_queue.front().find(':') != std::string::npos)
		postfix_flag = true;

	if (!split_msg.empty() && split_msg.front()[0] == ':')
	{
		this->_prefix = split_msg.front().replace(0, 1, "");
		split_msg.pop();
	}
	if (!split_msg.empty())
	{
		this->_command = split_msg.front();
		split_msg.pop();
	}

	while (!split_msg.empty())
	{
		if (postfix_flag && split_msg.front()[0] != ':')
		{
			if (split_msg.front()[0] == ',')
				split_msg.front().replace(0, 1, "");
			this->_postfix.push_back(split_msg.front());
		}
		else
		{
			this->_parameters.push_back(split_msg.front());
			postfix_flag = false;
		}
		split_msg.pop();
	}

	if (_parameters.front()[0] == ':')
		_parameters.front().replace(0, 1, "");

	joinString(this->_postfix, _postfix_str);
	joinString(this->_parameters, _parameters_str);

	_message_queue.pop();
}

void Message::joinString(std::vector<std::string>& dst, std::string& src)
{
	for (int i = 0; i < dst.size(); i++)
	{
		if (!src.empty())
			src += " ";
		src += dst[i];
	}
}

// Getter
const std::string	&Message::getCommand() const { return _command; }
const std::string	&Message::getPrefix() const { return _prefix; }
const std::vector<std::string>& Message::getPostfix() const { return _postfix; }
const std::string &Message::getPostfixStr() const { return _postfix_str; }
const std::vector<std::string>	&Message::getParams() const { return _parameters; }
const std::string &Message::getParamsStr() const { return _parameters_str; }
std::queue<std::string> Message::getMessageQueue() { return _message_queue; }

void Message::clearData()
{
	_prefix.clear();
	_postfix.clear();
	_postfix_str.clear();
	_parameters.clear();
	_parameters_str.clear();
	_answer_for_client.clear();
}

std::string &Message::getAnswerForClient() { return _answer_for_client; }

void Message::setAnswerForClient(const std::string& answer) { _answer_for_client = answer; }
