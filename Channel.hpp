#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include "Utils.hpp"

class Channel
{
 private:
	std::string _channel_name;
	std::string _channel_password;
	// TODO: Для простого удаления канала если он пустой. Стоит ли юзать unsigned?
	int _count_users_in_channel;

	std::map<int, std::string> _channel_user_fd_map;
	std::map<std::string, User *> _channel_user_nick_map;
	std::map<std::string, int> _channel_users;

	std::vector<std::string> _channel_operators;
	std::vector<std::string> _channel_ban_list;
	std::vector<bool> _channel_mode;

 public:
//	Channel(const std::string& name);
	Channel(const std::string& name, const std::string& pass);
	~Channel();

	std::map<std::string, int> &getChannelUsers();
	std::map<std::string, User *> &getChannelUserNickMap();
	std::vector<std::string> &getChannelOperators();

	std::string &getChannelName();
	std::string &getChannelPassword();

	void setChannelName();
	void setChannelPassword();
	void setChannelMode(int i, bool status);

	int& getCountUsers();

	void setCountUsersPlus();

	void setCountUsersMinus();
};

#endif //CHANNEL_HPP
