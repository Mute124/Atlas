/**************************************************************************************************
 * @file Window.h
 * 
 * @brief Everything required to create a window. If you wish to create a window, you may use
 * the GLFWGameWindow or the SDLGameWindow classes direction (if you dont wish to make your
 * own window class). If you want to use a custom window class, you should inherit from
 * IGameWindow or NullGameWindow, depending on your needs.
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <type_traits>
#include <any>
#include <variant>

#include "../../core/Core.h" // this include has to be put here because the GLFW_INCLUDE_VULKAN is defined in this file (of course if vulkan and GLFW3 is being used!)
#include "../../core/Common.h"

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>

#elif defined ATLAS_USE_SDL2
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_video.h>
#endif // ATLAS_USE_GLFW3


#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Atlas {

	/**
	 * @brief A using alias for a GLM low-precision 2D vector that can be used to store the size
	 * of a window.
	 * 
	 * @note There is no need for any real amount of precision as this is just used for storing
	 * the size of the window, so having a higher precision is not necessary and could be a
	 * performance hit.
	 *
	 * @since v0.0.1
	 */
	using WindowSize = glm::vec<2, uint32_t, glm::lowp>;
	
	using WindowRectangle = Rectangle<int16_t>;

	struct WindowHint {
		std::string identifier;
		std::string value;
	};

	/**
	 * @brief A struct that contains all the information that a window needs. 
	 * 
	 * @since v
	 */
	struct WindowDescription {

		std::string title;
		std::string iconPath;

		WindowRectangle windowRectangle;

		uint16_t targetFPS;
	};

	/**
	 * @brief An interface that allows the creation of custom window classes in
	 * an API-agnostic way. It contains some general pure virtual functions that
	 * are required to create a window. **This interface is what Atlas stores
	 * a pointer as when creating a window.**
	 * 
	 * @note If you want to make your own window class and you don't want to use
	 * the @ref NullGameWindow class, you should inherit from this interface instead.
	 * 
	 * @since v0.0.1
	 */
	class IGameWindow {
	public:

		virtual ~IGameWindow() = default;

		virtual void init(const uint32_t cInitFlags) = 0;

		virtual void update() = 0;

		virtual void open(const uint32_t cOpenFlags) = 0;

		virtual bool shouldClose() = 0;

		virtual void close() = 0;

		virtual void cleanup() = 0;

		virtual bool isOpen() const = 0;

		virtual bool isInitialized() const = 0;

		virtual void setHint(WindowHint const& hint) = 0;

		virtual void setIcon(std::string const& newIconPath) = 0;

		virtual void setWindowTitle(std::string const& newWindowTitle) = 0;

		virtual void setWindowPosition(int16_t newX, int16_t newY) = 0;

		virtual void setWindowSize(int16_t newWidth, int16_t newHeight) = 0;

		virtual void setTargetFPS(int16_t newTargetFPS) = 0;

		virtual WindowDescription getWindowDescription() const = 0;

		//explicit virtual operator WindowDescription() const = 0;
	};

	/**
	 * @brief A null, non-copyable implementation of a game window. It does not do anything as it only provides stubs.
	 * Furthermore, this class is also used as the base class for the GLFWGameWindow and SDLGameWindow classes.
	 * 
	 * @since v0.0.1
	 */
	class NullGameWindow : public IGameWindow {
	protected:

		static inline bool sHasSetGraphicsAPIFlag = false;

		std::string mWindowTitle;
		std::string mIconPath;

		WindowRectangle mWindowRect;

		int16_t mTargetFPS;

		bool mIsInitialized = false;

		bool mIsOpen = false;
	public:

		static inline uint32_t sGraphicsAPIFlag = -1;

		using IGameWindow::IGameWindow;

		NullGameWindow(NullGameWindow const&) = delete;

		NullGameWindow& operator=(NullGameWindow const&) = delete;

		explicit NullGameWindow(const uint32_t cGraphicsAPIFlag);

		~NullGameWindow() override = default;

		WindowDescription getWindowDescription() const override;


		//explicit operator WindowDescription() const override;
	};


#ifdef ATLAS_USE_GLFW3

	struct GameWindowSettings {
		bool enableEventPolling = true;
		bool fullscreen = false;

		GLFWmonitor* monitor = nullptr; 
	};

	class GLFWGameWindow final : public NullGameWindow {
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

		uint32_t windowInitFlags;
	};

	class SDLGameWindow : public NullGameWindow {
	private:
		std::unordered_map<uint32_t, std::function<int(SDL_Event const&, SDLGameWindow&)>> mEventHandlers;

		SDL_Window* mSDLWindowPointer = nullptr;

		uint64_t mFrameCount = 0;

		uint32_t mSDLWindowFlags = 0;

		bool mShouldClose = false;

		/**
		 * @brief Checks if an event occurred by simply calling SDL_PollEvents with a nullptr.
		 * 
		 * @return @a True if SDL_PollEvents does not return 0, otherwise @a false.
		 * 
		 * @since v
		 */
		static inline bool EventOccurred();

		static inline uint32_t GetGraphicsAPIFlag();

	public:
		explicit SDLGameWindow(const uint32_t cGraphicsAPIFlag);

		SDLGameWindow() = default;

		~SDLGameWindow() override = default;

		// Inherited via NullGameWindow
		void init(const uint32_t cInitFlags) override;

		void update() override;

		void open(const uint32_t cOpenFlags) override;

		bool shouldClose() override;

		void close() override;

		void cleanup() override;

		bool isOpen() const override;

		bool isInitialized() const override;

		void setHint(WindowHint const& hint) override;

		void setIcon(std::string const& newIconPath) override;

		void setWindowTitle(std::string const& newWindowTitle) override;

		void setWindowPosition(int16_t newX, int16_t newY) override;

		void setWindowSize(int16_t newWidth, int16_t newHeight) override;

		void setTargetFPS(int16_t newTargetFPS) override;
	};

#endif // ATLAS_USE_GLFW3

	unsigned int GetWindowConfigFlag(std::string const& flagName);
}
