#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <vector>
#include <any>
#include <string>
#include <cstdint>

#include "../core/Core.h"

#include "IOCommon.h"
#include "PathSearcher.h"



namespace Atlas {
	


	// This is like a filter for various options that may happen during the file discovery process
	class IPathLoaderLogicGate {
	public:

		IPathLoaderLogicGate() = default;

		virtual ~IPathLoaderLogicGate() = default;

		virtual bool shouldLoadOnDiscovery(PathLocation const& cPath) = 0;
		
		virtual bool shouldLoadPath(PathLocation const& cPath) = 0;
	};

	class PathLoaderLogicGate : public IPathLoaderLogicGate {
	private:

	public:

		bool shouldLoadOnDiscovery(PathLocation const& cPath) override { return true; }

		bool shouldLoadPath(PathLocation const& cPath) override { return true; }
	};

	class AExtensionValidator {
	private:

	public:
	
		bool isValid(PathLocation const& cPath) const { return true; }
	};

	

	class IOManager {
	private:
		
		struct ExtensionHandlersAggregator {
			std::unique_ptr<AExtensionValidator> extensionValidator;
			//std::unique_ptr<AParser> parser;
		};

		std::unordered_map<std::string, ExtensionHandlersAggregator> mExtensionHandlers;

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
