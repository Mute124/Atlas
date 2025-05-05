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

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>
#elif defined(ATLAS_USE_SDL2)

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_video.h>
	#include <SDL2/SDL_events.h>
#endif

#include <iostream>

Atlas::AGameWindow::AGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon)
	: IGameWindow(), mWindowTitle(title), mWindowWidth(width), mWindowHeight(height), mWindowConfigFlags(windowConfigFlags), mTargetFPS(targetFPS), mWindowIcon(icon) {
}

std::string const& Atlas::AGameWindow::getTitle() const { return mWindowTitle; }

uint32_t Atlas::AGameWindow::getWidth() const { return mWindowWidth; }

uint32_t Atlas::AGameWindow::getHeight() const { return mWindowHeight; }

unsigned int Atlas::AGameWindow::getFlags() const { return mWindowConfigFlags; }

unsigned int Atlas::AGameWindow::getTargetFPS() const { return mTargetFPS; }

bool Atlas::AGameWindow::isOpen() const { return mIsOpen; }

bool Atlas::AGameWindow::isInitialized() const { return mIsInitialized; }

bool Atlas::AGameWindow::shouldClose() { return mShouldClose; }

#ifdef ATLAS_USE_GLFW3

Atlas::GLFWGameWindow::GLFWGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings)
	: AGameWindow(title, width, height, windowConfigFlags, targetFPS, icon), mGameWindowSettings(gameWindowSettings)
{
}

Atlas::GLFWGameWindow::~GLFWGameWindow() {
	close(true); 
}

void Atlas::GLFWGameWindow::init()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
	
	this->mIsInitialized = true;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);	
}

void Atlas::GLFWGameWindow::open()
{
	if (this->mGameWindowSettings.monitor == nullptr && this->mGameWindowSettings.fullscreen) {
		this->mGameWindowSettings.monitor = glfwGetPrimaryMonitor();
	}

	this->mGLFWWindowPointer = glfwCreateWindow(this->mWindowWidth, this->mWindowHeight, this->mWindowTitle.c_str(), this->mGameWindowSettings.monitor, nullptr);

	if (this->mGLFWWindowPointer == nullptr || glfwGetError(nullptr) != GLFW_NO_ERROR) {
		throw std::runtime_error("Failed to create GLFW window");
	}
	else {
		this->mIsOpen = true;
	}
}

void Atlas::GLFWGameWindow::update()
{
	if (this->mGameWindowSettings.enableEventPolling) {
		glfwPollEvents();
	}
}

bool Atlas::GLFWGameWindow::shouldClose()
{
	return glfwWindowShouldClose(this->mGLFWWindowPointer);
}

void Atlas::GLFWGameWindow::close(bool shouldCleanup)
{
	if (this->mGLFWWindowPointer != nullptr) {
		glfwDestroyWindow(this->mGLFWWindowPointer);
	}

	// in order to simplify the deinitialization process, this function may also call cleanup.
	if (shouldCleanup) {
		this->cleanup();
	}
}

void Atlas::GLFWGameWindow::cleanup()
{
	glfwTerminate();
}

void Atlas::GLFWGameWindow::setFlag(std::string const& flagName, unsigned int value)
{
	unsigned int flag = GetWindowConfigFlag(flagName);

	glfwWindowHint(flag, value);

	const char* errorString = nullptr;
	int error = glfwGetError(&errorString);
	
	if(error != GLFW_NO_ERROR) {
		throw std::runtime_error(errorString);
	}
}


#elif defined(ATLAS_USE_SDL2)

bool Atlas::SDLGameWindow::EventOccurred()
{
	// Since the documentation of SDL_PollEvent states that this function will return whether or not there is a 
	// pending event in the event queue if the parameter is null, we can safely ignore the return value here.
	return SDL_PollEvent(nullptr) != 0;
}

Atlas::SDLGameWindow::SDLGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings)
: AGameWindow(title, width, height, windowConfigFlags, targetFPS, icon), mGameWindowSettings(gameWindowSettings)
{
}

Atlas::SDLGameWindow::~SDLGameWindow()
{
}

void Atlas::SDLGameWindow::init()
{
	ATLAS_ASSERT((!this->mIsInitialized && this->mSDLWindowPointer != nullptr), "Window is already initialized! Please call close() and/or cleanup() first before initializing another window.");

	const int cInitResult = SDL_Init(SDL_INIT_VIDEO);

	if (cInitResult != 0) {
		throw std::runtime_error("Failed to initialize SDL");
	}
	else {
		this->mIsInitialized = true;
	}
}

void Atlas::SDLGameWindow::open()
{
	this->mSDLWindowPointer = SDL_CreateWindow(this->mWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->mWindowWidth, this->mWindowHeight, this->mGameWindowSettings.windowInitFlags);

	if (this->mSDLWindowPointer == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}
	else {
		this->mIsOpen = true;
	}
}

void Atlas::SDLGameWindow::update()
{
	SDL_Event sdlEvent;

	// Since we only care about doing event checking IF an event exists, this variable is used to track that.
	bool eventsExist = EventOccurred();

	if (!eventsExist) {
		// if no events exist, we can skip the rest of the function.

		std::cout << "No events exist, skipping window update" << std::endl;
		return;
	}

	while(eventsExist) {
		SDL_PollEvent(&sdlEvent);

		this->mEventHandlers[sdlEvent.type](sdlEvent, *this);

		eventsExist = EventOccurred();
	}
}

bool Atlas::SDLGameWindow::shouldClose()
{
	return this->mShouldClose;
}

void Atlas::SDLGameWindow::close(bool shouldCleanup)
{
	if (this->mSDLWindowPointer != nullptr) {
		SDL_DestroyWindow(this->mSDLWindowPointer);
	}

	// in order to simplify the deinitialization process, this function may also call cleanup.
	if (shouldCleanup) {
		this->cleanup();
	}
}

void Atlas::SDLGameWindow::cleanup()
{
	SDL_Quit();
}

void Atlas::SDLGameWindow::setFlag(std::string const& flagName, unsigned int value)
{
}

#endif // ATLAS_USE_GLFW3

unsigned int Atlas::GetWindowConfigFlag(std::string const& flagName)
{
	unsigned int result = 0;

#ifdef ATLAS_USE_GLFW3

	if (GLFWGameWindow::sWindowFlagsTranslationMap.contains(flagName)) {
		result = GLFWGameWindow::sWindowFlagsTranslationMap.at(flagName);
	}
	else {
		throw std::runtime_error("Unknown flag: " + flagName);
	}

#endif // ATLAS_USE_GLFW3

	return result;
}