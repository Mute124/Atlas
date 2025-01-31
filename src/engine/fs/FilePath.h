#pragma once
#include <string>

namespace Atlas {	
	/// <summary>
	/// This class represents a file path. Since the file system is sandboxed, this contains the <b>full path</b> and the <b>sandboxed path</b>. A sandboxed path is a 
	/// relative to the "game" dir. For example, if a file was in the "game/data" folder, the path would be "data". This is to make usage of the file system much easier. When 
	/// constructed, the sandboxed path is automatically resolved. 
	/// <para>
	/// A full path is the exact location on the disk. This should NOT be exposed to the modding API as it is extremely dangerous and makes it easy for malicious mods to attack.
	/// </para>
	/// </summary>
	class FilePath {
	private:
		std::string mFullPath; 

		// Instead of the full path, this is the path relative to the "game" dir. For example, if a file was in the "game/data" folder, this would be "data".
		std::string mSandboxPath;
	public:
		
		/// <summary>
		/// Gets the full path.
		/// </summary>
		/// <returns>The exact location on the disk</returns>
		std::string getFullPath() const;
		
		/// <summary>
		/// Gets the sandboxed path. A sandboxed path is a path relative to the "game" dir. For example, if a file was in the "game/data" folder, the path would be "data".
		/// </summary>
		/// <returns></returns>
		std::string getSandboxPath() const;
		
		/// <summary>
		/// Gets the full sandbox path.
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <returns></returns>
		const std::string getFullSandboxPath(std::string const& filename) const;
		
		/// <summary>
		/// Gets the full sandbox path.
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <param name="extension">The extension.</param>
		/// <returns></returns>
		const std::string getFullSandboxPath(std::string const& filename, std::string const& extension) const;
	};

	/// <summary>
	/// Checks if a path is within the sandbox by checking [path] against the current
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool IsPathSandboxed(std::string const& path);
}