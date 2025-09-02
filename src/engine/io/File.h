#pragma once
#include <any>

#include "IOCommon.h"

namespace Atlas {


	template<typename T_DATA_STORAGE_TYPE>
	class AFile {
	private:
		bool mIsLoaded = false;
		bool mIsParsed = false;
		bool mIsOpen = false;
		
		PathLocation mFilePath;

		std::any mData;

		uint64_t mUUID = 0;

		uint32_t mSize = 0;
	public:

		virtual void load() {
			mIsLoaded = true;
		}

		virtual void parse() {
			mIsParsed = true;
		}
	};
}
