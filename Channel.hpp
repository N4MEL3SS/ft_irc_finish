#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Utils.hpp"

class Channel
{
 private:
	std::string _channel_name;
	std::string _channel_password;

	std::map<int, std::string> _channel_user_fd_map;
	std::map<std::string, User *> _channel_user_nick_map;

	std::vector<std::string> _channel_users;
//	std::map<std::string, std::vector<bool> > _channel_users_mode;
	std::vector<std::string> _channel_operators;
	std::vector<std::string> _channel_ban_list;
	std::vector<bool> _channel_mode;

 public:
//	Channel(const std::string& name);
	Channel(const std::string& name, const std::string& pass);
	~Channel();

	std::vector<std::string> &getChannelUsers();
	std::vector<std::string> &getChannelOperators();
	std::map<std::string, User *> &getChannelUserNickMap();

	std::string &getChannelName();
	std::string &getChannelPassword();

	void setChannelName();
	void setChannelPassword();

	void setChannelMode(int i, bool status);
};

#endif //CHANNEL_HPP
