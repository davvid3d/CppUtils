#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <chrono>

namespace genericTools
{
	std::vector<std::string> split(const std::string &s, char delimiter);
	std::string trim(const std::string &str, const std::string &whitespace = " \t");
	std::string reduce(const std::string &str, const std::string &fill = " ", const std::string &whitespace = " \t");

	uint64_t millis();
	uint64_t micros();
	uint64_t nanos();

	struct tm convertTimestampToDate(uint64_t &timestamp);
	struct tm convertMillissTimestampToDate(uint64_t &timestamp_ms);
	struct tm convertMicrosTimestampToDate(uint64_t &timestamp_us);
	struct tm convertNanosTimestampToDate(uint64_t &timestamp_ns);

	uint64_t convertDateToTimestamp(struct tm &date);
	uint64_t convertDateToMillisTimestamp(struct tm &date);
	uint64_t convertDateToMicrosTimestamp(struct tm &date);
	uint64_t convertDateToNanosTimestamp(struct tm &date);

}