#pragma once
#include <iostream>
#include <map>

#define CONFIGURATION_DEFAULT_FILE_PATH "./config.txt"
#define CONFIGURATION_DEFAULT_DELIMITER "="
#define CONFIGURATION_DEFAULT_COMMENT "#"

namespace genericTools
{
	class configuration
	{
		class Param
		{
		public:
			enum Status
			{
				MAP,
				VALUE
			};

		public:
			Param();
			Param(std::string const &value);
			Param(std::map<std::string, Param> const &map);

			operator bool() const;
			operator char() const;
			operator short() const;
			operator int() const;
			operator long() const;
			operator double() const;
			operator float() const;
			operator std::string() const;

			Param &operator=(bool);
			Param &operator=(char);
			Param &operator=(short);
			Param &operator=(int);
			Param &operator=(long);
			Param &operator=(double);
			Param &operator=(float);
			Param &operator=(std::string const &);
			Param &operator=(const char[]);

			Param &operator[](std::string const &);
			Param &operator[](const char[]);

		private:
			Param::Status _status;
			std::string _value;
			std::map<std::string, Param> _map;
		};

	public:
		configuration();
		virtual ~configuration();

		configuration::Param &operator[](std::string const &);
		configuration::Param &operator[](const char[]);

		bool loadConfig(std::string fileName = CONFIGURATION_DEFAULT_FILE_PATH);
		void dumpConfig();

	protected:
		void loadDefaultConfig();
		std::string trim(const std::string &str, const std::string &whitespace = " \t");
		std::string reduce(const std::string &str, const std::string &fill = " ", const std::string &whitespace = " \t");

	private:
		std::map<std::string, Param> _params;
	};
}
