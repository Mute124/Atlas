#pragma once
#include <cstdint>
#include <memory>
#include "FileMeta.h"

namespace Atlas {
	class File {
	private:
		/**
		 * @brief A flag indicating if the file has been loaded
		 * @remarks @c true if this instance is loaded; @c false otherwise. This is set to @c false by default.
		 * @since v0.0.9
		 */
		bool mIsLoaded = false;

		/**
		 * @brief 
		 */
		std::shared_ptr<FileMeta> mFileMeta = nullptr;
	public:

		/**
		 * @brief Returns if this file is still just a registered file or actually loaded and usable by the code.
		 * @return 
		 */
		bool isLoaded() const;
		void setLoadState(bool state);

		std::shared_ptr<FileMeta> getFileMeta() const;
	};
}