#ifndef USER_HPP
#define USER_HPP

#include "Utils.hpp"

class User
{
 private:
	// TODO: Чекнуть регистр ника
	// Входные данные при регистрации. Могут быть изменены позднее
	std::string _nickname;
	std::string _username;
	std::string _realname;
	// Fullname для сервера формата "nick!name@localhost"
	std::string _fullname;

	std::map<std::string, char> _user_channel_mode;
	// Файловый дескриптор пользователя
	int _user_fd;

	// Флаг для прерванных сообщение
	bool _is_partial_message;
	// Переменная для хранения прерванных сообщений
	std::string _partial_message;

	// Подключен ли пользователь
	bool _is_connected;
	// Зарегистрирован ли пользователь
	bool _is_registered;

	// Является ли пользователь IRC оператором канала
	bool _is_irc_operator;

	// Находится ли пользователь в сети
	bool _is_online;

	User();

 public:
	User(int user_fd);
	~User();

	// Getter
	bool getRegistrationStatus() const;
	bool getConnectionStatus() const;
	bool getIrcOperatorStatus() const;

	bool getIsPartialMessage() const;
	std::string getPartialMessage() const;

	int getUserFD() const;
	std::string getNickName();
	std::string getUserName();
	std::string getFullName();
	std::string getRealName();

	// Setter
	void setIsPartialMessage(bool condition);
	void setPartialMessage(const std::string& message);

	void setIrcOperatorStatus(bool condition);
	void setConnectionStatus(bool condition);
	void setRegistrationStatus(bool condition);

	void setNickName(const std::string& name);
	void setUserName(const std::string& name);
	void setRealName(const std::string& name);
	void setFullName(const std::string& nick, const std::string& name);

	std::map<std::string, char> getUserChannelMode() const;

	bool getOnlineStatus() const;

	void setOnlineStatus(bool condition);
};

#endif //USER_HPP
