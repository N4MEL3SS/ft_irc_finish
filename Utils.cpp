#include "Utils.hpp"

std::string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

std::queue<std::string> split(const std::string &str, char sep)
{
	std::queue<std::string> ret;
	std::string::const_iterator	i = str.begin();

	while(i != str.end())
	{
		while (i != str.end() && *i == sep)
			i++;

		std::string::const_iterator	j = std::find(i, str.end(), sep);
		if (i != str.end())
		{
			ret.push(std::string(i, j));
			// TODO: Можно ли вынести из if
			i = j;
		}
	}

	return ret;
}

std::map<std::string, std::string> MakeConfigMap()
{
	std::map<std::string, std::string> config_map;

	config_map["server_name"] = "server";
	config_map["info"] = "none";
	config_map["admin_name"] = "";
	config_map["admin_nickname"] = "";
	config_map["admin_email"] = "";
	config_map["operator_password"] = "1";
	config_map["max_channels"] = "5";

	return config_map;
}

void FillConfigMap(std::map<std::string, std::string> &config_map, const std::string& temp)
{
	std::string key;
	std::string value;
	size_t pos;

	if ((pos = temp.find_first_of(':')) != std::string::npos)
	{
		size_t trim = temp.find_first_not_of(" \t");
		key = temp.substr(trim, pos - trim);
		value = temp.substr(temp.find_first_not_of(" \t", pos + 1));
		config_map[key] = value;
	}
}

void FillConf(std::map<std::string, std::string> &config_map, config_file &conf)
{
	conf.admin_email = config_map["admin_email"];
	conf.admin_name = config_map["admin_name"];
	conf.admin_nickname = config_map["admin_nickname"];
	conf.info = config_map["info"];
	conf.max_channels = std::atoi(config_map["max_channels"].c_str());
	conf.operator_password = config_map["operator_password"];
	conf.server_name = config_map["server_name"];
}

config_file parseConfigFile(const std::string& path)
{
	std::ifstream conf_file;
	std::stringstream ss;
	std::map<std::string, std::string> config_map = MakeConfigMap();
	std::string temp;
	config_file conf;

	conf_file.open(path.c_str());
	if (!conf_file.is_open())
		return conf;
	ss << conf_file.rdbuf();

	while (getline(ss, temp, '\n'))
	{
		try
		{
			FillConfigMap(config_map, temp);
		}
		catch(const std::exception& e){}
	}

	FillConf(config_map, conf);
	return conf;
}

void printConfigFileFields(const config_file& config)
{
	// TODO: Поискать другой цвет
	std::cout << GRAY;
	std::cout << "__________________ ===Server config=== __________________\n";
	std::cout << "Server name: " << config.server_name << '\n';
	std::cout << "Info: " << config.info << '\n';
	std::cout << "Admin name: "  << config.admin_name << '\n';
	std::cout << "Admin nickname: " << config.admin_nickname << '\n';
	std::cout << "Admin email: " << config.admin_email << '\n';
	std::cout << "Operator password doesn't show up due to security reasons\n";
	std::cout << "Maximum channels on server: " << config.max_channels << '\n';
	std::cout << "---------------------------------------------------------\n";
	std::cout << RESET << std::endl;
}

void sendToClient(int fd, std::string &msg)
{
	msg += "\r\n";
	send(fd, msg.c_str(), msg.size(), IRC_NOSIGNAL);

	if (std::count(msg.begin(), msg.end(), '\n') > 1)
		std::cout << YELLOW << "Send to client [" << fd << "]\n" << RESET;
	else
		std::cout << YELLOW << "Send to client [" << fd << "]: " << RESET;
	replaceCRLF(msg);
	std::cout << BLUE << msg + "\n" << RESET << std::endl;
}

void replaceCRLF(std::string& msg)
{
	while (msg.find("\r\n") != std::string::npos)
		msg.replace(msg.find("\r\n"), 2, "\n");
}
