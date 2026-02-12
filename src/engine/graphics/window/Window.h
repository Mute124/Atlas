/**************************************************************************************************
 * @file Window.h
 * 
 * @brief Everything required to create a window. If you wish to create a window, you may use
 * the GLFWGameWindow or the GameWindow classes direction (if you dont wish to make your
 * own window class). If you want to use a custom window class, you should inherit from
 * GameWindow or NullGameWindow, depending on your needs.
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
#include <array>

#include "../../core/Core.h" // this include has to be put here because the GLFW_INCLUDE_VULKAN is defined in this file (of course if vulkan and GLFW3 is being used!)
#include "../../core/Common.h"
	
#include "../../debugging/AException.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/qualifier.hpp>
#include <SDL2/SDL_events.h>
#include <vulkan/vulkan_core.h>

#include <eventpp/eventqueue.h>

namespace Atlas {
	enum class EWindowFlag : uint32_t {
		Fullscreen = 0,
		Shown,
		Hidden,
		Borderless,
		Resizable,
		Minimized,
		Maximized,
		HighDPI,
		AlwaysOnTop
	};

	enum class EWindowFlashCondition : uint8_t {
		Cancel = 0,
		Briefly,
		Until_Focused
	};

	class InvalidWindowRectException : public AException {
	public:
		using AException::AException;
	};

	class WindowInitFailureException : public AException {
	public:
		using AException::AException;
	};

	struct WindowRect {
		glm::ivec2 position{ 800, 600 };
		glm::ivec2 size{ 800, 600 };

		WindowRect(glm::ivec2 const& position, glm::ivec2 const& size) : position(position), size(size) {
			if (size.x < 0 || size.y < 0) {
				throw InvalidWindowRectException("Window size cannot be negative");
			}
		}

		WindowRect(int32_t x, int32_t y, int32_t width, int32_t height) : WindowRect({ x, y }, { width, height }) {}

		WindowRect() = default;
		
		~WindowRect() = default;

		int getWidth() const { return size.x; }
		int getHeight() const { return size.y; }
	};

	struct WindowDescription {
		WindowRect windowRect{};
		std::string windowTitle{ "Atlas Window" };

		WindowDescription(const WindowRect& windowRect, const std::string& windowTitle)
			: windowRect(windowRect), windowTitle(windowTitle)
		{
		}

		WindowDescription() = default;
	};

	class GameWindow final : public Initializable {
	private:
		WindowDescription mWindowDescription{};
		SDL_Window* mSDLWindowPointer{ nullptr };
		bool mbIsVisible{ false };
		bool mbQuitRequested{ false };

		static inline uint32_t ComputeWindowFlags(std::vector<EWindowFlag> const& windowFlags);

		static inline SDL_Window* CreateSDLWindow(WindowDescription const& windowDescription, uint32_t windowFlags);

		static inline void DestroySDLWindow(SDL_Window* window);

	public:

		ATLAS_EXPLICIT GameWindow(WindowDescription const& windowDescription);

		GameWindow() : GameWindow(WindowDescription{}) {};

		~GameWindow() override;

		void open(uint32_t windowFlags);

		/**
		* @brief Opens the window with the given window flags by first computing the window flags into a single uint32_t value
		* and then passing it to the open function that takes a single uint32_t.
		*
		* @param[in] windowFlags A vector of window flags (EWindowFlag) to open the window with
		*
		* @since v0.0.1
		*/
		void open(std::vector<EWindowFlag> const& windowFlags);

		void close();

		void hide();

		void show();
		
		void update();

		void flash(const EWindowFlashCondition flashCondition);

		void setOpacity(float newOpacity);

		float getOpacity() const;

		bool shouldClose() const;

		bool isOpen() const;

		bool isVisible() const;

		WindowDescription getWindowDescription() const;

		SDL_Window* getWindowHandle();
	};

	unsigned int GetWindowConfigFlag(std::string const& flagName);
}
