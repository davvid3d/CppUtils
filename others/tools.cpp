#include "tools.h"

using namespace genericTools;

std::vector<std::string> genericTools::split(const std::string &s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string genericTools::trim(const std::string &str, const std::string &whitespace)
{
	size_t strBegin = str.find_first_not_of(whitespace);
	// reuturn empty if it is all whitespace
	if (strBegin == std::string::npos)
		return "";

	size_t strEnd = str.find_last_not_of(whitespace);
	size_t strRange = strEnd - strBegin + 1;
	// remove multiple whitespaces
	return str.substr(strBegin, strRange);
}

std::string genericTools::reduce(const std::string &str, const std::string &fill, const std::string &whitespace)
{
	// trim first
	std::string result = trim(str, whitespace);

	// replace sub ranges
	size_t beginSpace = result.find_first_of(whitespace);
	while (beginSpace != std::string::npos)
	{
		size_t endSpace = result.find_first_not_of(whitespace, beginSpace);
		size_t range = endSpace - beginSpace;

		result.replace(beginSpace, range, fill);

		size_t newStart = beginSpace + fill.length();
		beginSpace = result.find_first_of(whitespace, newStart);
	}

	return result;
}

// Get time stamp in milliseconds.
uint64_t genericTools::millis()
{
	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
					  std::chrono::system_clock::now().time_since_epoch())
					  .count();

	return ms;
}

// Get time stamp in microseconds.
uint64_t genericTools::micros()
{
	uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
					  std::chrono::system_clock::now().time_since_epoch())
					  .count();

	return us;
}

// Get time stamp in nanoseconds.
uint64_t genericTools::nanos()
{
	uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
					  std::chrono::system_clock::now().time_since_epoch())
					  .count();

	return ns;
}

tm genericTools::convertTimestampToDate(uint64_t &timestamp)
{
	std::time_t time = (time_t)(timestamp);
	struct tm convertedDate;
	localtime_s(&convertedDate, &time);

	return convertedDate;
}

tm genericTools::convertMillissTimestampToDate(uint64_t &timestamp_ms)
{
	std::time_t time = (time_t)(timestamp_ms / 1e+3);
	struct tm convertedDate;
	localtime_s(&convertedDate, &time);

	return convertedDate;
}

tm genericTools::convertMicrosTimestampToDate(uint64_t &timestamp_us)
{
	std::time_t time = (time_t)(timestamp_us / 1e+6);
	struct tm convertedDate;
	localtime_s(&convertedDate, &time);

	return convertedDate;
}

tm genericTools::convertNanosTimestampToDate(uint64_t &timestamp_ns)
{
	std::time_t time = (time_t)(timestamp_ns / 1e+9);
	struct tm convertedDate;
	localtime_s(&convertedDate, &time);

	return convertedDate;
}

uint64_t genericTools::convertDateToTimestamp(tm &date)
{
	return (uint64_t)mktime(&date);
}

uint64_t genericTools::convertDateToMillisTimestamp(tm &date)
{
	return (uint64_t)(mktime(&date) * 1e+3);
}

uint64_t genericTools::convertDateToMicrosTimestamp(tm &date)
{
	return (uint64_t)(mktime(&date) * 1e+6);
}

uint64_t genericTools::convertDateToNanosTimestamp(tm &date)
{
	return (uint64_t)(mktime(&date) * 1e+9);
}
