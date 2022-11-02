#include "User.hpp"

User::User(int user_fd): _user_fd(user_fd)
{
	_is_connected = true;
	_is_registered = false;
	_is_irc_operator = false;
	_nickname = "";
	_username = "";
	_realname = "";
}

User::~User() {}

// Getter
bool User::getConnectionStatus() const { return this->_is_connected; }
bool User::getRegistrationStatus() const { return this->_is_registered; }
bool User::getIrcOperatorStatus() const { return this->_is_irc_operator; }

bool User::getIsPartialMessage() const { return this->_is_partial_message; }
std::string User::getPartialMessage() const { return this->_partial_message; }

std::map<std::string, char> User::getUserChannelMode() const {return this->_user_channel_mode; }

int User::getUserFD() const { return this->_user_fd; }
std::string User::getNickName() { return this->_nickname; }
std::string User::getUserName() { return this->_username; }


// Setter
void User::setIsPartialMessage( bool condition ) { this->_is_partial_message = condition; }
void User::setPartialMessage(const std::string& message) { this->_partial_message = message; }
void User::setIrcOperatorStatus(){ this->_is_irc_operator = true; }

void User::setConnectionStatus(bool condition) { this->_is_connected = condition; }
void User::setRegistrationStatus(bool condition) { this->_is_registered = condition; }

void User::setNickName(const std::string& name) { this->_nickname = name; }
void User::setUserName(const std::string& name) { this->_username = name; }
void User::setRealName(const std::string& name) { this->_realname = name; }
