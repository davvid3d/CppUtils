#pragma once
#include <iostream>
#include <sstream>
#include <mutex>
#include <fstream>
#include <string>
#include <filesystem>

/*
*	This logger is thread free
* 	it can be used with std::endl
*	use as following LOG << "blabla" << ...
*/

static class logger
{
public:
	template <typename T>
	static inline void log(T &message)
	{
		mutex.lock();
		//std::cout << messageSTR;
		if (logFile_.is_open())
		{
			if (message.str() == "" && writeDate_)
			{
				logFile_ << getCurrentTime();
			}
			logFile_ << message.str();
			logFile_.flush(); //force flush ofstream
		}
		message.flush();
		mutex.unlock();
	}
	static inline void setPath(std::string &path, bool writeDate = true)
	{
		writeDate_ = writeDate;
		std::filesystem::path pathToCreate(path);
		std::filesystem::create_directories(pathToCreate);
		logFile_.open(pathToCreate.string() + "/" + "log.txt", std::ios::app);
	}
	static inline void end()
	{
		logFile_.close();
	}

protected:
	static inline std::string getCurrentTime()
	{
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S : ", &timeinfo);

		return std::string(buffer);
	}

private:
	inline static std::mutex mutex;
	inline static std::ofstream logFile_;
	inline static bool writeDate_;
} LOG;

struct loggerBuffer
{
	std::stringstream ss;

	//delete default constructors to force use move to prevent overflow
	loggerBuffer() = default;
	loggerBuffer(const loggerBuffer &) = delete;
	loggerBuffer &operator=(const loggerBuffer &) = delete;
	loggerBuffer &operator=(loggerBuffer &&) = delete;

	loggerBuffer(loggerBuffer &&buf) noexcept : ss(move(buf.ss))
	{
	}
	template <typename T>
	loggerBuffer &operator<<(T &&message)
	{
		ss << std::forward<T>(message);
		return *this;
	}

	loggerBuffer &operator<<(std::ostream &(*var)(std::ostream &))
	{
		ss << var;
		return *this;
	}

	~loggerBuffer()
	{
		LOG.log(ss);
	}
};

template <typename T>
loggerBuffer operator<<(logger &simpleLogger, T &&message)
{
	loggerBuffer buf;
	buf.ss << std::forward<T>(message);
	return buf;
}