#pragma once
#include <string>

namespace Atlas {
	class Language {
	public:
		std::string language; // Ex. English, French
		std::string region; // Ex. United States, United Kingdom
		std::string regionCode; // Ex. US, GB
		std::string code; // Ex. en-US, en-GB. If not present, it will set it to the filename
	};
}