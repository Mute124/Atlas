/**************************************************************************************************
 * @file Window.cpp
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <cstdint>

#include "../../core/Core.h"
#include "../../core/Common.h"

#include "Window.h"
#include "../../debugging/Logging.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

#include <imgui/backends/imgui_impl_sdl2.h>

#ifdef ATLAS_USE_VULKAN
	#include "../vulkan/VKDevice.h"
#endif

Atlas::WindowRect::WindowRect(glm::ivec2 const& position, glm::ivec2 const& size) : position(position), size(size) {
	if (size.x < 0 || size.y < 0) {
		throw InvalidWindowRectException("Window size cannot be negative");
	}
}

int Atlas::WindowRect::getWidth() const {
	return size.x; 
}

int Atlas::WindowRect::getHeight() const {
	return size.y; 
}

Atlas::WindowDescription::WindowDescription(const WindowRect& windowRect, const std::string& windowTitle)
	: windowRect(windowRect), windowTitle(windowTitle)
{
}

inline uint32_t Atlas::GameWindow::ComputeWindowFlags(std::vector<EWindowFlag> const& windowFlags) {
	uint32_t flags = 0;

	for (auto const& flag : windowFlags) {
		flags |= static_cast<uint32_t>(flag);
	}

	return flags;
}

inline SDL_Window* Atlas::GameWindow::CreateSDLWindow(WindowDescription const& windowDescription, uint32_t windowFlags) {
	const std::string cWindowFlags = std::format("0x{:X}", windowFlags);
	const std::string cWindowTitle = std::format("\"{}\"", windowDescription.windowTitle);
	const std::string cWindowPosition = std::format("{}, {}", windowDescription.windowRect.position.x, windowDescription.windowRect.position.y);
	const std::string cWindowSize = std::format("{}, {}", windowDescription.windowRect.size.x, windowDescription.windowRect.size.y);
	const std::string cVideoDriverName = std::format("\"{}\"", SDL_GetCurrentVideoDriver());

	const std::string cLogString = std::format("Creating SDL window\n\t-> Flags: {}\n\t-> Title: {}\n\t-> Position: {}\n\t-> Size: {}\n\t-> Video Driver: {}", cWindowFlags, cWindowTitle, cWindowPosition, cWindowSize, cVideoDriverName);
	InfoLog(cLogString);

	return SDL_CreateWindow(
		windowDescription.windowTitle.c_str(),
		windowDescription.windowRect.position.x,
		windowDescription.windowRect.position.y,
		windowDescription.windowRect.size.x,
		windowDescription.windowRect.size.y,
		windowFlags
	);
}

inline void Atlas::GameWindow::DestroySDLWindow(SDL_Window* window) {
	// No need to destroy a null window
	if (window == nullptr) {
		return;
	}

	InfoLog("Destroying window");

	SDL_DestroyWindow(window);
}

Atlas::GameWindow::GameWindow(WindowDescription const& windowDescription) : Initializable(false), mWindowDescription(windowDescription) {
	const int cInitResult = SDL_Init(SDL_INIT_EVERYTHING);

	if (cInitResult < 0) {
		const std::string cSDLError = std::string(SDL_GetError());
		const std::string cErrorString = std::format("Failed to initialize SDL video subsystem\n\tError: {}\n\tError Code: {}", cSDLError, cInitResult);

		throw WindowInitFailureException(cErrorString);
	}

	this->setInit();
}

Atlas::GameWindow::~GameWindow() {
	this->setNotInit();

	if (mSDLWindowPointer != nullptr) {
		DestroySDLWindow(mSDLWindowPointer);

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
}

void Atlas::GameWindow::open(uint32_t windowFlags) {
	mSDLWindowPointer = CreateSDLWindow(getWindowDescription(), windowFlags);

	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}
}

void Atlas::GameWindow::open(std::vector<EWindowFlag> const& windowFlags) {
	if (!windowFlags.empty()) {
		uint32_t flags = ComputeWindowFlags(windowFlags);

		open(flags);
	}
}

void Atlas::GameWindow::close() {
	if (mSDLWindowPointer != nullptr) {
		DestroySDLWindow(mSDLWindowPointer);
	}

	mbQuitRequested = true;
	this->setNotInit();
}

void Atlas::GameWindow::hide() {
	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("SDL window pointer is null");
	}

	SDL_HideWindow(mSDLWindowPointer);
	mbIsVisible = false;
}

void Atlas::GameWindow::show() {
	if (mSDLWindowPointer == nullptr) {
		throw std::runtime_error("SDL window pointer is null");
	}

	SDL_ShowWindow(mSDLWindowPointer);

	mbIsVisible = true;
}

void Atlas::GameWindow::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			close();
		}

		if (event.type != SDL_QUIT) {

			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}
}

void Atlas::GameWindow::flash(const EWindowFlashCondition flashCondition) {
	if (mSDLWindowPointer != nullptr) {
		SDL_FlashOperation translatedFlashCondition;

		switch (flashCondition)
		{
			using enum EWindowFlashCondition;

		case Cancel:
			translatedFlashCondition = SDL_FLASH_CANCEL;
			break;

		case Briefly:
			translatedFlashCondition = SDL_FLASH_BRIEFLY;
			break;

		case Until_Focused:
			translatedFlashCondition = SDL_FLASH_UNTIL_FOCUSED;
			break;

		default:
			translatedFlashCondition = SDL_FLASH_CANCEL;
			break;
		}

		SDL_FlashWindow(mSDLWindowPointer, translatedFlashCondition);
	}
}

void Atlas::GameWindow::setOpacity(float newOpacity)
{
	if (mSDLWindowPointer != nullptr) {
		InfoLog(std::format("Setting window opacity to {}", newOpacity));

		SDL_SetWindowOpacity(mSDLWindowPointer, newOpacity);
	}
}

float Atlas::GameWindow::getOpacity() const
{
	float opacity = 0.0f;

	if (mSDLWindowPointer != nullptr) {
		SDL_GetWindowOpacity(mSDLWindowPointer, &opacity);
	}

	return opacity;
}

bool Atlas::GameWindow::shouldClose() const {
	//return SDL_PollEvent(nullptr) == 0;
	return mSDLWindowPointer == nullptr || mbQuitRequested;
}

bool Atlas::GameWindow::isOpen() const { 
	return mSDLWindowPointer != nullptr;
}

bool Atlas::GameWindow::isVisible() const { 
	return mbIsVisible;
}

Atlas::WindowDescription Atlas::GameWindow::getWindowDescription() const { 
	return mWindowDescription;
}

SDL_Window* Atlas::GameWindow::getWindowHandle() { 
	return mSDLWindowPointer;
}
