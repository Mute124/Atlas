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
#endif





Atlas::AGameWindow::AGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon)
	: IGameWindow(), mWindowTitle(title), mWindowWidth(width), mWindowHeight(height), mWindowConfigFlags(windowConfigFlags), mTargetFPS(targetFPS), mWindowIcon(icon) {
}

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

	while(SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_QUIT) {
			this->mShouldClose = true;
		}
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