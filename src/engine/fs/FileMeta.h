#pragma once
#include <string>
#include "FilePath.h"

namespace Atlas {	
	/// <summary>
	/// File meta is the attributes of a registered/loaded file. This is also used to search for files within the file system.  
	/// </summary>
	/// <remarks>
	/// File meta contains the extension, path, sandboxed path, and filename and is created once a file is registered during the discovery process. 
	/// </remarks>
	class FileMeta {
	public:
		// IMPORTANT: Searching by extension is highly discouraged because there can be multiple files with the same extension. Only use this if you know what you are doing.
		/// <summary>
		/// The extension
		/// </summary>
		std::string extension;
		
		/// <summary>
		/// The path
		/// </summary>
		std::string path;
		
		/// <summary>
		/// The sandbox path
		/// </summary>
		std::string sandboxPath;
		
		/// <summary>
		/// The filename
		/// </summary>
		std::string filename;
		
		/// <summary>
		/// Resolves the file's extension from the filename.
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <returns></returns>
		std::string resolveExtension(std::string const& filename);
		
		/// <summary>
		/// Resolves the filename.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns></returns>
		std::string resolveFilename(std::string const& path);
		
		/// <summary>
		/// Resolves the sandbox path.
		/// </summary>
		/// <param name="path">The path.</param>
		/// <returns></returns>
		std::string resolveSandboxPath(std::string const& path);
		
		/// <summary>
		/// Initializes a new instance of the <see cref="FileMeta"/> class.
		/// </summary>
		FileMeta();
		
		/// <summary>
		/// Initializes a new instance of the <see cref="FileMeta"/> class.
		/// </summary>
		/// <param name="path">The path.</param>
		FileMeta(std::string const& path);
		
		/// <summary>
		/// Gets the full path.
		/// </summary>
		/// <returns></returns>
		FilePath getFullPath();		

		/// <summary>
		/// Gets the sandbox path.
		/// </summary>
		/// <returns></returns>
		FilePath getSandboxPath();
		
		/// <summary>
		/// Gets the full sandbox path.
		/// </summary>
		/// <returns></returns>
		FilePath getFullSandboxPath();

		std::string format();
	};
}