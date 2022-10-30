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
			++i;

		std::string::const_iterator	j = std::find(i, str.end(), sep);
		if (i != str.end())
		{
			/*
			if (include && j != str.end())
				ret.push_back(std::string(i, j + 1));
			else
				ret.push_back(std::string(i, j));
			*/

			ret.push(std::string(i, j));
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

void FillConfigMap(std::map<std::string, std::string> &config_map, std::string temp)
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
	std::ifstream conf_file(path);
	std::stringstream ss;
	std::map<std::string, std::string> config_map = MakeConfigMap();
	std::string temp;
	config_file conf;

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
