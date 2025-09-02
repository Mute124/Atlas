/**************************************************************************************************
 * @file PathSearcher.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <algorithm>
#include <string>
#include <regex>
#include <functional>

#include "PathSearcher.h"
//
//Atlas::APathSearcher::APathSearcher(std::unique_ptr<IFileSystemIndexer> cFileSystemIndexer) : mFileSystemIndexer(std::move(cFileSystemIndexer)) {
//}

Atlas::APathSearcher::~APathSearcher() {
	mIgnoredRegexPatterns.clear();
}

void Atlas::APathSearcher::setRootSearchPath(const PathLocation& cPath) {
	mRootSearchPath = cPath;

	if (mRootSearchPath.empty()) {
		mRootSearchPath = PathLocation(".");
	}

	mRootSearchPathSet = true;
}

void Atlas::APathSearcher::addIgnoredRegexPattern(std::regex const& cPattern) {
	mIgnoredRegexPatterns.push_back(cPattern);
	
}

void Atlas::APathSearcher::addIgnoredRegexPatterns(std::vector<std::regex> const& cPatterns)
{
	for (auto const& cPattern : cPatterns) {
		addIgnoredRegexPattern(cPattern);
	}
}

void Atlas::APathSearcher::addFoundPath(FoundPathResult const& cFoundPath)
{
}

void Atlas::APathSearcher::clearIgnoredRegexPatterns() { 
	mIgnoredRegexPatterns.clear(); 
}

bool Atlas::APathSearcher::shouldIgnoreItem(const std::string& cItemName) const
{
	// Check if the item name matches any of the ignored regex patterns
	return std::any_of(mIgnoredRegexPatterns.begin(),
		mIgnoredRegexPatterns.end(),
		[&cItemName](std::regex const& cPattern) { 
			return std::regex_match(cItemName, cPattern); 
		}
	);
}

Atlas::PathLocation Atlas::APathSearcher::getRootSearchPath() { 
	return mRootSearchPath;
}

void Atlas::RecursivePathSearcher::search(const PathLocation& cPath)
{
	for (auto const& cDirEntry : std::filesystem::directory_iterator(cPath)) {
		if (cDirEntry.is_directory()) {
			search(cDirEntry.path());
		}
		else if (!shouldIgnoreItem(cDirEntry.path().filename().string())) {
			FoundPathResult foundPathResult;

			foundPathResult.path = cDirEntry.path();
			
			if (cDirEntry.is_directory()) {
				// No need to manually set foundPathResult.isFile as it is automatically set to false
				// by the FoundPathResult constructor
				foundPathResult.isDirectory = true;
				foundPathResult.pathType = EPathType::Directory;
			} else {
				// No need to manually set foundPathResult.isDirectory as it is automatically set to false
				// by the FoundPathResult constructor
				foundPathResult.isFile = true;
				foundPathResult.pathType = EPathType::File;
			}

			addFoundPath(foundPathResult);

			//mFoundPaths.push_back(cDirEntry.path());
		}
	}
}

void Atlas::RecursivePathSearcher::search() {
	for (auto const& cDirEntry : std::filesystem::directory_iterator(getRootSearchPath())) {

		if (shouldIgnoreItem(cDirEntry.path().filename().string())) {
			continue;
		}



		if (cDirEntry.is_directory()) {
			search(cDirEntry.path());
		}
	}
}
