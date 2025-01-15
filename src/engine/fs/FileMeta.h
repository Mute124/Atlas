#pragma once
#include <string>

namespace Atlas {
	class FileMeta {
	public:
		// IMPORTANT: Searching by extension is highly discouraged because there can be multiple files with the same extension. Only use this if you know what you are doing.
		std::string extension;
		std::string path;
		std::string filename;

		FileMeta();
	};
}