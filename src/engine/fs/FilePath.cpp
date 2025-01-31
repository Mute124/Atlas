#include "FilePath.h"
#include "../Common.h"

bool Atlas::IsPathSandboxed(std::string const& path)
{
	// make sure the path is not at a higher level than

	return false;
}

inline std::string Atlas::FilePath::getFullPath() const { return mFullPath; }

inline std::string Atlas::FilePath::getSandboxPath() const { return mSandboxPath; }

inline const std::string Atlas::FilePath::getFullSandboxPath(std::string const& filename) const { return mSandboxPath + "/" + filename; }

inline const std::string Atlas::FilePath::getFullSandboxPath(std::string const& filename, std::string const& extension) const { return mSandboxPath + "/" + filename + extension; }
