#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

namespace Atlas {
	// ------------------------------------------------------------------
	//	Time Utils
	// ------------------------------------------------------------------
/*
	template<typename T>
	concept Clock = std::is_same_v(T, std::chrono::system_clock) || std::is_same_v(T, std::chrono::steady_clock) || std::is_same_v(T, std::chrono::high_resolution_clock) || std::is_same_v(T, std::chrono::monotonic_clock);
*/

/*	class Timestamp {
	public:

		Timestamp() {
			std::time_t t = std::time(nullptr);
			std::tm* now = std::localtime(&t);
			
			mTime = std::to_string(now->tm_year + 1900) + "-" +
				std::to_string(now->tm_mon + 1) + "-" +
				std::to_string(now->tm_mday) + " " +
				std::to_string(now->tm_hour) + ":" +
				std::to_string(now->tm_min) + ":" +
				std::to_string(now->tm_sec);

		}

		time_t getTime() const {
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}


	private:
		 std::string mTime;

	};*/

	std::string GetDate();
}