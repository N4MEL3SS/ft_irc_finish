#include "Channel.hpp"

//Channel::Channel(const std::string& name) : _channel_name(name) {}
Channel::Channel(const std::string& name, const std::string& pass) : _channel_name(name), _channel_password(pass)
{
	_channel_mode.assign(CHANNEL_MODE_SIZE, false);
}

Channel::~Channel() {}

std::map<std::string, int> &Channel::getChannelUsers() { return this->_channel_users; }
std::map<std::string, User *> &Channel::getChannelUserNickMap() { return this->_channel_user_nick_map; }

std::string& Channel::getChannelPassword()
{
	return this->_channel_password;
}

void Channel::setChannelMode(int i,bool status) { _channel_mode[i] = status; }

std::vector<std::string>& Channel::getChannelOperators() { return this->_channel_operators; }
