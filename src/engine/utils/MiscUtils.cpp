#include "MiscUtils.h"

std::string Techstorm::GetDate() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	return std::to_string(now->tm_year + 1900) + "-" +
		std::to_string(now->tm_mon + 1) + "-" +
		std::to_string(now->tm_mday) + " " +
		std::to_string(now->tm_hour) + ":" +
		std::to_string(now->tm_min) + ":" +
		std::to_string(now->tm_sec);
}
