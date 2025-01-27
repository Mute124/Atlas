#pragma once
#include <string>

namespace Atlas {	
	/// <summary>
	/// 
	/// </summary>
	class FilePath {
	private:
		std::string mFullPath;

		// Instead of the full path, this is the path relative to the "game" dir. For example, if a file was in the "game/data" folder, this would be "data".
		std::string mSandboxPath;
	public:

		std::string getFullPath() const;
		std::string getSandboxPath() const;

		const std::string getFullSandboxPath(std::string const& filename) const;
		const std::string getFullSandboxPath(std::string const& filename, std::string const& extension) const;
	};

	/// <summary>
	/// Checks if a path is within the sandbox by checking [path] against the current
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool IsPathSandboxed(std::string const& path);
}