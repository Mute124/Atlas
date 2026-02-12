/**************************************************************************************************
 * @file Platform.h
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <string>
#include <memory>
#include <stdexcept>

#include "../../io/IOManager.h"
#include "../Core.h"
#include "../threading/ThreadBudget.h"

namespace Atlas {
	struct ApplicationInfo {
		std::string applicationName;

		explicit ApplicationInfo(const std::string &applicationName) : applicationName(applicationName) {}
		ApplicationInfo() = default;
	};

	struct PlatformInitInfo {
		ApplicationInfo applicationInfo;
		FileManager::Options fileManagerOptions;
		ThreadBudget threadBudget;
	};

	class IPlatform {
	public:
		virtual ~IPlatform() = default;

		virtual ThreadBudget getThreadBudget() const = 0;
		virtual ApplicationInfo getApplicationInfo() const = 0;
		virtual std::weak_ptr<FileManager> getFileManager() const = 0;
	};

	class Platform : public IPlatform {
	private:
		const ApplicationInfo mApplicationInfo;

		std::shared_ptr<FileManager> mFileManager;
		ThreadBudget mThreadBudget;
	public:

		explicit Platform(const PlatformInitInfo &initInfo)
			: IPlatform(), mApplicationInfo(initInfo.applicationInfo), mFileManager(std::make_shared<FileManager>(initInfo.fileManagerOptions))
		{
			if (mFileManager == nullptr) {
				throw std::runtime_error("Failed to create file manager");
			}
		}

		ThreadBudget getThreadBudget() const final {
			return mThreadBudget;
		}

		ApplicationInfo getApplicationInfo() const final { 
			return mApplicationInfo;
		}

		std::weak_ptr<FileManager> getFileManager() const final {
			return mFileManager;
		}
	};

#ifdef ATLAS_USE_SDL2
	class SDL2Platform {
	private:
		//struct SDL2InitInfo {
		//	bool bInitEverything{ false };
		//	bool bInitVideo{ false };
		//	bool bInitAudio{ false };
		//	bool bInitEvents{ false };
		//	bool bInitPhysics{ false };
		//	bool bInitScripting{ false };
		//	bool bInitModding{ false };

		//	uint32_t initFlags = 0;

		//	ATLAS_EXPLICIT SDL2InitInfo(bool bInitEverything);
		//};

	public:


		
	};
#endif
}
