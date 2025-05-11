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

	/**
	 * @brief A struct that contains all the information that a window needs. 
	 * 
	 * @since v
	 */
	struct WindowDescription {

		std::string windowTitleString;
		std::string iconPath;

		WindowRectangle windowRectangle;

		uint16_t targetFPS;
		
		bool isInitialized = false;

		bool isOpen = false;

		/**
		 * @brief A map that contains all the window configuration flags.
		 * 
		 * @note This map has a key of type uint32_t because this is supposed to be what is actually
		 * passed to the windowing API. GLFW and SDL2 both use uint32_t as the flag identifier and
		 * this is the reason for this. In other words, the key is a lower-level flag identifier.
		 * 
		 * @since v0.0.1
		 */
		std::unordered_map<uint32_t, std::variant<std::string, uint32_t>> windowConfigFlags;
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

		virtual void init() = 0;

		virtual void update() = 0;

		virtual void open() = 0;

		virtual bool shouldClose() = 0;

		virtual void close() = 0;

		virtual void cleanup() = 0;

		virtual bool isOpen() const = 0;

		virtual bool isInitialized() const = 0;

		virtual void setFlag(uint32_t flagIdentifier, uint32_t newValue) = 0;

		virtual void setFlag(uint32_t flagIdentifier, std::string const& newValue) = 0;

		virtual void setIcon(std::string const& newIconPath) = 0;

		virtual WindowDescription& getWindowDescription() = 0;
	};

	/**
	 * @brief A null, non-copyable implementation of a game window. It does not do anything as it only provides stubs.
	 * Furthermore, this class is also used as the base class for the GLFWGameWindow and SDLGameWindow classes.
	 * 
	 * @since v0.0.1
	 */
	class NullGameWindow : public IGameWindow {
	private:
		static inline uint32_t sGraphicsAPIFlag = -1;
		static inline bool sHasSetGraphicsAPIFlag = false;

		WindowDescription mWindowDescription;

	protected:

		explicit NullGameWindow(WindowDescription const& windowDescription, const uint32_t cGraphicsAPIFlag);

	public:
		
		using IGameWindow::IGameWindow;

		NullGameWindow(NullGameWindow const&) = delete;

		NullGameWindow& operator=(NullGameWindow const&) = delete;

		explicit NullGameWindow(WindowDescription const& windowDescription);

		~NullGameWindow() override = default;

		void init() override;

		void open() override;

		void close() override;

		bool isOpen() const override;

		bool isInitialized() const override;

		void setFlag(uint32_t flagIdentifier, uint32_t newValue) override;

		void setFlag(uint32_t flagIdentifier, std::string const& newValue) override;

		void setIcon(std::string const& newIconPath) override;

		WindowDescription& getWindowDescription() noexcept final;

		uint32_t getGraphicsAPIFlag() const noexcept;
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

	class SDLGameWindow final : public NullGameWindow {
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
		explicit SDLGameWindow(WindowDescription const& windowDescription, const uint32_t cGraphicsAPIFlag);

		explicit SDLGameWindow(WindowDescription const& windowDescription);

		~SDLGameWindow() = default;

		// Inherited via NullGameWindow
		void init() override;

		void update() override;

		void open() override;

		bool shouldClose() override;

		void close() override;

		void cleanup() override;

		void setFlag(uint32_t flagIdentifier, uint32_t newValue) override;

		void setFlag(uint32_t flagIdentifier, std::string const& newValue) override;

		void setIcon(std::string const& newIconPath) override;
	};

#endif // ATLAS_USE_GLFW3

	unsigned int GetWindowConfigFlag(std::string const& flagName);
}
