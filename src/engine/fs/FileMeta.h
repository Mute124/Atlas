#pragma once
#include <string>
#include "FilePath.h"

namespace Atlas {
	class FileMeta {
	private:



	public:
		// IMPORTANT: Searching by extension is highly discouraged because there can be multiple files with the same extension. Only use this if you know what you are doing.
		std::string extension;
		std::string path;
		std::string sandboxPath;
		std::string filename;

		std::string resolveExtension(std::string const& filename);
		std::string resolveFilename(std::string const& path);
		std::string resolveSandboxPath(std::string const& path);
		FileMeta();

		FileMeta(std::string const& path);

		FilePath getFullPath();
		FilePath getSandboxPath();
		FilePath getFullSandboxPath(); 
	};
}