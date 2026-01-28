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
#include <vector>
#include <unordered_set>

#include "../Common.h"

namespace Atlas {

	enum class EModuleType {
		Everything = 0,
		Video,
		Audio,
		Input,
		Networking,
		Haptics,
		Events,
		Physics,
		Debug,
		Scripting,
		Modding
	};

	class APlatform : public Singleton<APlatform> {
	public:
		struct Options {
			////struct EnabledModules {
			////	bool bInitEverything{ false };
			////	bool bInitVideo{ false };
			////	bool bInitAudio{ false };
			////	bool bInitEvents{ false };
			////	bool bInitPhysics{ false };
			////	bool bInitScripting{ false };
			////	bool bInitModding{ false };
			////};
			//struct ThreadingOptions {
			//	
			//	bool bEnableMultithreading{ false };
			//	uint8_t threadCountAllowance{ 1 };
			//};
			//std::unordered_set<EModuleType> enabledModules{};

			//ThreadingOptions threadingOptions{};

			//uint32_t initFlags = 0;

			struct LoggingOptions {
				std::string logFilePath;
				std::string loggerName;
				std::string messageFormatPattern;

				bool bTruncateMessages{ false };
			};

			LoggingOptions loggingOptions{};
		};
	private:
		
		Options mOptions;

	protected:
		
		void setOptions(Options const& options) { mOptions = options; }

	public:

		explicit APlatform(Options const& options) : mOptions(options) {}

		APlatform::Options const& getOptions() const { return mOptions; }
	};

#ifdef ATLAS_USE_SDL2
	class SDL2Platform : public APlatform {
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

		explicit SDL2Platform(Options const& options);

		void initLogging();

		void initSDL2(uint32_t sdlInitFlags);

		
	};
#endif
}
