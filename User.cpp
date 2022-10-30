#include "User.hpp"

User::User(int user_fd): _user_fd(user_fd)
{
	_is_connected = true;
	_is_registered = false;
	_registered_stage = 0;
	_nickname = "";
	_username = "";
	_realname = "";
}

User::~User() {}

// Getter
bool User::getConnectionStatus() const { return this->_is_connected; }
int User::getRegistrationStage() const { return this->_registered_stage; }
bool User::getRegistrationStatus() const { return this->_is_registered; }
bool User::getIsPartialMessage() const { return this->_is_partial_message; }
std::string User::getPartialMessage() const { return this->_partial_message; }

// Setter
void User::setIsPartialMessage( bool condition ) { this->_is_partial_message = condition; }
void User::setPartialMessage(const std::string& message) { this->_partial_message = message; }
void User::setRegistrationStage(int var) { this->_registered_stage = var; }
void User::zeroRegistrationStage() { this->_registered_stage = 0; }
void User::incRegistrationStage() { this->_registered_stage++; }
