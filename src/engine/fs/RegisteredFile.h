#pragma once
#include <any>
#include <memory>
#include <mutex>

#include "FileMeta.h"

namespace Atlas {
	class RegisteredFile {
	public:
		bool isLoaded = false;
		std::any data = nullptr;
		std::shared_ptr<FileMeta> meta = nullptr;
		uint16_t index = 0;
		std::mutex loadMutex = std::mutex();

		RegisteredFile();
		explicit RegisteredFile(std::shared_ptr<FileMeta> const& meta);

		template<typename T>
		T get() { return std::any_cast<T>(data); }
	};
}