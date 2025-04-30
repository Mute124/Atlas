#pragma once

#include <string>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>

#include "../../core/Core.h" // this include has to be put here because the GLFW_INCLUDE_VULKAN is defined in this file (of course if vulkan and GLFW3 is being used!)

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>

#elif defined ATLAS_USE_SDL2
	#include <SDL2/SDL.h>
#endif // ATLAS_USE_GLFW3


namespace Atlas {


	class IGameWindow {
	protected:
		std::string mWindowTitle;
		std::string mWindowIcon;

		uint32_t mWindowWidth;
		uint32_t mWindowHeight;
		
		unsigned int mWindowConfigFlags;
		unsigned int mTargetFPS;

		bool mIsOpen = false;
		bool mIsInitialized = false;

	public:
		IGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon);

		virtual void init() = 0;

		virtual void open() = 0;

		virtual void update() = 0;

		virtual bool shouldClose() = 0;

		virtual void close(bool shouldCleanup) = 0;

		virtual void cleanup() = 0;

		virtual void setFlag(std::string const& flagName, unsigned int value) = 0;
	};

#ifdef ATLAS_USE_GLFW3

	struct GameWindowSettings {
		bool enableEventPolling = true;
		bool fullscreen = false;

		GLFWmonitor* monitor = nullptr; 
	};

	class GLFWGameWindow final : public IGameWindow {
	private:

		friend unsigned int GetWindowConfigFlag(std::string const& flagName);

		/**
		 * @brief A static and read-only translation map. It takes an input string of the flag name and returns the GLFW flag value
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref setFlag() for one of the functions that this map is used in.
		 */
		static inline const std::unordered_map<std::string, uint16_t> sWindowFlagsTranslationMap = {
			{ "Resizable", GLFW_RESIZABLE },
			{ "Visible", GLFW_VISIBLE },
			{ "Decorated", GLFW_DECORATED },
			{ "Focused", GLFW_FOCUSED },
			{ "Auto Iconify", GLFW_AUTO_ICONIFY },
			{ "Floating", GLFW_FLOATING },
			{ "Maximized", GLFW_MAXIMIZED },
			{ "Centered", GLFW_CENTER_CURSOR },
			{ "Transparent", GLFW_TRANSPARENT_FRAMEBUFFER },
			{ "Focus", GLFW_FOCUS_ON_SHOW },
			{ "Scale to Monitor", GLFW_SCALE_TO_MONITOR }
		};

		GLFWwindow* mGLFWWindowPointer = nullptr;

		GameWindowSettings mGameWindowSettings;


	public:

		GLFWGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings);

		virtual ~GLFWGameWindow();

		// Inherited via IWindow
		void init() override;

		void open() override;

		void update() override;

		bool shouldClose() override;

		void close(bool shouldCleanup) override;

		void cleanup() override;

		void setFlag(std::string const& flagName, unsigned int value) override;
	};

#elif defined(ATLAS_USE_SDL2)

	struct GameWindowSettings {
		bool enableEventPolling = true;
		bool fullscreen = false;

	};

	class SDLGameWindow final : public IGameWindow {
	private:
		GameWindowSettings mGameWindowSettings;
	public:
		SDLGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings);

		virtual ~SDLGameWindow();

		// Inherited via IWindow
		void init() override;

		void open() override;

		void update() override;

		bool shouldClose() override;

		void close(bool shouldCleanup) override;

		void cleanup() override;
	};
#endif // ATLAS_USE_GLFW3

	unsigned int GetWindowConfigFlag(std::string const& flagName);
}
