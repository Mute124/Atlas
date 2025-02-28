#pragma once
#include <memory>
#include <mutex>

#include "FileMeta.h"
#include "RegisteredFile.h"
#include <any>

namespace Atlas {
	class LoadedFile : public RegisteredFile {
	private:
		std::any mFileData = nullptr;
	public:
	};
}