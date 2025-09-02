/**************************************************************************************************
 * @file FileSystemMap.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <filesystem>
#include <unordered_map>
#include <regex>
#include "../core/Core.h"

#include "PathSearcher.h"

namespace Atlas {

	enum class ELookupType {
		Extension,
		Filename,
		FullPath
	};

	

	class FileSystemMap {
	private:
		union PathSearchResult {

		};

		std::unordered_map<std::string, PathSearchResult> mFileSystemMap;
	public:


		void addPathSearchResult(std::string const& cPath, PathSearchResult const& cPathSearcherResult) {
			mFileSystemMap[cPath] = cPathSearcherResult;
		}

		PathSearchResult getPathSearchResult(std::string const& cKey) const {
			return mFileSystemMap.at(cKey);
		}
	};
}
