#ifndef USER_HPP
#define USER_HPP

#include "Utils.hpp"

class User
{
 private:
	// Входные данные при регистрации. Могут быть изменены позднее
	std::string _nickname;
	std::string _username;
	std::string _realname;

	// Файловый дескриптор пользователя
	int _user_fd;

	// прерванное ли сообщение
	// TODO: Возможно лишнее
	bool _is_partial_message;
	// Переменная для хранения прерванных сообщений
	std::string _partial_message;

	// Массив каналов в которых пользователь является оператором
	// TODO: Возможно лишнее
	std::map<std::string, bool> _operator_in_channels;

	// Вектор каналов в которых пользователь состоит
	// TODO: Возможно лишнее
	std::vector<std::string> _include_in_channels;

	// Подключен ли пользователь
	bool _is_connected;
	int _registered_stage;
	// Зарегистрирован ли пользователь
	bool _is_registered;
	// Является ли пользователь администратором
	bool _is_server_admin;


	User();

 public:
	User(int _user_fd);
	~User();

	// Getter
	int getRegistrationStage() const;
	bool getRegistrationStatus() const;
	bool getConnectionStatus() const;
	bool getIsPartialMessage() const;

	std::string getPartialMessage() const;
	// Setter
	void setIsPartialMessage(bool condition);
	void setPartialMessage(const std::string& message);
	void setRegistrationStage(int var);
	void zeroRegistrationStage();
	void incRegistrationStage();
};

#endif //USER_HPP
