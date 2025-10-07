/**************************************************************************************************
 * @file PathSearcher.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "IOCommon.h"

namespace Atlas {

	enum class EPathType : int8_t
	{
		Unknown = -1,
		File,
		Directory
	};

	struct FoundPathResult {
		bool isDirectory = false;
		bool isFile = false;

		EPathType pathType = EPathType::Unknown;
		PathLocation path;
	};

	struct PathSearchResult {

	};

	class APathSearcher {
	private:
		PathLocation mRootSearchPath;

		std::vector<std::regex> mIgnoredRegexPatterns;

		bool mRootSearchPathSet = false;
	
		std::vector<FoundPathResult> mFoundPaths;
	public:

		APathSearcher() = default;

		//explicit APathSearcher(std::unique_ptr<IFileSystemIndexer> cFileSystemIndexer);

		virtual ~APathSearcher();

		virtual void search() = 0;
	
		void setRootSearchPath(const PathLocation& cPath);
		
		void addIgnoredRegexPattern(std::regex const& cPattern);

		void addIgnoredRegexPatterns(std::vector<std::regex> const& cPatterns);

		virtual void addFoundPath(FoundPathResult const& cFoundPath);

		void clearIgnoredRegexPatterns();

		bool shouldIgnoreItem(const std::string& cItemName) const;

		PathLocation getRootSearchPath();

		std::vector<FoundPathResult> getFoundPaths() const { return mFoundPaths; }

		//PathLocation getFullPath(const PathLocation& path) { return mRootSearchPath / path; }
	};

	class RecursivePathSearcher : public APathSearcher {
	public:
		void search(const PathLocation& cPath);
		
		void search() override;
	};
}
