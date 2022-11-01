#include "Channel.hpp"

Channel::Channel(const std::string& name) : _channel_name(name) {}
Channel::Channel(const std::string& name, const std::string& pass) : _channel_name(name), _channel_password(pass) {}

Channel::~Channel() {}

std::vector<std::string>& Channel::getChannelUsers() { return this->_channel_users; }
std::vector<std::string>& Channel::getChannelOperators() { return this->_channel_operators; }
std::map<std::string, User *> &Channel::getChannelUserNickMap() { return this->_channel_user_nick_map; }

std::string& Channel::getChannelPassword()
{
	return this->_channel_password;
}
