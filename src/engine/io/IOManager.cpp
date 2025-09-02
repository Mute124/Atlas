#include <memory>
#include <filesystem>
#include <type_traits>

#include "IOManager.h"
#include "PathSearcher.h"
#include "IOCommon.h"

Atlas::IOManager::IOManager(const PathLocation& cExecutablePath, const PathLocation& cCurrentWorkingDirectory) : mExecutablePath(cExecutablePath), mCurrentWorkingDirectory(cCurrentWorkingDirectory) {
}

void Atlas::IOManager::setExecutablePath(const PathLocation& cExecutablePath) {
	mExecutablePath = cExecutablePath; 
}

void Atlas::IOManager::setCurrentWorkingDirectory(const PathLocation& cCurrentWorkingDirectory) { 
	mCurrentWorkingDirectory = cCurrentWorkingDirectory;
}

void Atlas::IOManager::setPathSearcher(std::unique_ptr<APathSearcher> cPathSearcher)
{

	if (cPathSearcher == nullptr) {
		return;
	}
	
	mPathSearcher = std::move(cPathSearcher);
}

void Atlas::IOManager::search(APathSearcher* cPathSearcher)
{
	if (cPathSearcher == nullptr) {
		return;
	}

	cPathSearcher->search();

	mFoundPaths = cPathSearcher->getFoundPaths();
}

void Atlas::IOManager::search()
{
	search(mPathSearcher.get());
}

Atlas::PathLocation Atlas::IOManager::getExecutablePath() const { 
	return mExecutablePath; 
}

Atlas::PathLocation Atlas::IOManager::getCurrentWorkingDirectory() const { 
	return mCurrentWorkingDirectory; 
}

