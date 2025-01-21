#pragma once
#include <string>

namespace Atlas {	
	/// <summary>
	/// 
	/// </summary>
	class FilePath {
	private:
		std::string mFullPath;
		std::string mSandboxPath;
	public:

	};

	/// <summary>
	/// Checks if a path is within the sandbox by checking [path] against the current
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool IsPathSandboxed(std::string const& path);
}