#include "configuration.h"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace genericTools;

configuration::configuration()
{
}

genericTools::configuration::~configuration()
{
}

configuration::Param &configuration::operator[](const std::string &key)
{
	if (_params.find(key) == _params.end())
	{
		_params.insert(std::map<std::string, Param>::value_type(key, Param("")));
	}

	return (_params.find(key)->second);
}

configuration::Param &configuration::operator[](const char key[])
{
	return (this->operator[](std::string(key)));
}

configuration::Param::Param(const std::string &value)
	: _status(configuration::Param::Status::VALUE), _value(value)
{
}

configuration::Param::Param(const std::map<std::string, Param> &map)
	: _status(configuration::Param::Status::MAP), _map(map)
{
}

configuration::Param::Param() : _status(configuration::Param::Status::MAP)
{
}

configuration::Param &configuration::Param::operator[](const std::string &key)
{
	if (_map.find(key) == _map.end())
	{
		_map.insert(std::map<std::string, Param>::value_type(key, Param("")));
	}

	_status = configuration::Param::Status::MAP;

	return (_map.find(key)->second);
}

configuration::Param &configuration::Param::operator[](const char key[])
{
	return (this->operator[](std::string(key)));
}

configuration::Param &configuration::Param::operator=(bool value)
{
	_status = configuration::Param::Status::VALUE;

	_value = value ? "true" : "false";

	return (*this);
}

configuration::Param &configuration::Param::operator=(char value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(short value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(int value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(long value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(double value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(float value)
{
	_status = configuration::Param::Status::VALUE;

	_value = std::to_string(value);

	return (*this);
}

configuration::Param &configuration::Param::operator=(std::string const &value)
{
	_status = configuration::Param::Status::VALUE;

	_value = value;

	return (*this);
}

configuration::Param &configuration::Param::operator=(const char value[])
{
	_status = configuration::Param::Status::VALUE;

	_value = value;

	return (*this);
}

configuration::Param::operator bool(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (_value == "true");
}

configuration::Param::operator char(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (std::stoi(_value));
}

configuration::Param::operator short(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (std::stoi(_value));
}

configuration::Param::operator int(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (std::stoi(_value));
}

configuration::Param::operator long(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (std::stol(_value));
}

configuration::Param::operator double(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (std::stod(_value));
}

configuration::Param::operator float(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (static_cast<float>(std::stod(_value)));
}

configuration::Param::operator std::string(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	return (_value);
}

configuration::Param::operator std::vector<std::string>(void) const
{
	if (_status == configuration::Param::Status::MAP)
		throw std::runtime_error("Current index is not a valid value");

	std::vector<std::string> splittedVec = split(_value, (char)CONFIGURATION_DEFAULT_LIST_DELIMITER);

	//remove possible whitspaces
	for (size_t i = 0; i < splittedVec.size(); i++)
	{
		splittedVec[i] = reduce(splittedVec[i]);
	}

	return (splittedVec);
}

bool configuration::loadConfig(string fileName)
{
	ifstream inputFile;
	inputFile.open(fileName.c_str());
	string delimeter = CONFIGURATION_DEFAULT_DELIMITER;
	int initPos = 0;

	if (inputFile.fail())
	{
		cout << "Unable to find defaultConfig file" << endl;
		loadDefaultConfig();
		return false;
	}

	string line;
	while (getline(inputFile, line))
	{
		// Remove comment Lines
		size_t found = line.find_first_of(CONFIGURATION_DEFAULT_COMMENT);
		string configData = line.substr(0, found);

		// Remove return line from configData
		configData.erase(std::remove(configData.begin(), configData.end(), '\r'), configData.end());

		if (configData.empty())
			continue;

		unsigned int length = (unsigned int)configData.find(delimeter);

		string tag, value;

		if (length != string::npos)
		{
			tag = configData.substr(initPos, length);
			value = configData.substr((length + 1));
		}

		// Trim white spaces
		tag = reduce(tag);
		value = reduce(value);

		if (tag.empty() || value.empty())
			continue;

		// Check if any of the tags is repeated more than one times
		// it needs to pick the latest one instead of the old one.

		// Search, if the tag is already present or not
		// If it is already present, then delete an existing one

		std::map<std::string, Param>::iterator itr = _params.find(tag);
		if (itr != _params.end())
		{
			_params.erase(tag);
		}

		_params.insert(std::pair<string, string>(tag, value));
	}
	return true;
}

void genericTools::configuration::dumpConfig()
{
	map<string, Param>::iterator it;
	for (it = _params.begin(); it != _params.end(); ++it)
	{
		cout << it->first << " = " << it->second.operator std::string() << endl;
	}
}

void configuration::loadDefaultConfig()
{
	//TODO: set most basic acceptable configuration to run the application (hard coded ?)
}