#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "../core/Core.h"

#include "IOCommon.h"
#include "PathSearcher.h"


namespace Atlas {
	class IOManager {
	private:
		PathLocation mExecutablePath;
		PathLocation mCurrentWorkingDirectory;

		std::unique_ptr<APathSearcher> mPathSearcher;

		std::vector<FoundPathResult> mFoundPaths;
	public:

		IOManager() = default;

		explicit IOManager(const PathLocation& cExecutablePath, const PathLocation& cCurrentWorkingDirectory);

		void setExecutablePath(const PathLocation& cExecutablePath);

		void setCurrentWorkingDirectory(const PathLocation& cCurrentWorkingDirectory);
	
		void setPathSearcher(std::unique_ptr<APathSearcher> cPathSearcher);

		void search(APathSearcher* cPathSearcher);

		void search();



		PathLocation getExecutablePath() const;

		PathLocation getCurrentWorkingDirectory() const;
	};
}
