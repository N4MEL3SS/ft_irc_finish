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

	// Файловый дескриптор пользователя
	int _user_fd;

	// прервано ли сообщение
	bool _is_partial_message;
	// Переменная для хранения прерванных сообщений
	std::string _partial_message;

	// Подключен ли пользователь
	bool _is_connected;
	// Зарегистрирован ли пользователь
	bool _is_registered;

	User();

 public:
	User(int user_fd);
	~User();

	// Getter
	bool getRegistrationStatus() const;
	bool getConnectionStatus() const;

	bool getIsPartialMessage() const;
	std::string getPartialMessage() const;

	int getUserFD() const;
	std::string getNickName();
	std::string getUserName();


	// Setter

	void setIsPartialMessage(bool condition);
	void setPartialMessage(const std::string& message);

	void setConnectionStatus(bool condition);
	void setRegistrationStatus(bool condition);

	void setNickName(const std::string& name);
	void setUserName(const std::string& name);
	void setRealName(const std::string& name);
};

#endif //USER_HPP
