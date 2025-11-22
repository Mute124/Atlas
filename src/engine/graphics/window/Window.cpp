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
#include <iostream>

#include "../../core/Core.h"
#include "../../core/Common.h"

#include "Window.h"

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>
#elif defined(ATLAS_USE_SDL2)

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_stdinc.h>
	#include <SDL2/SDL_video.h>
	#include <SDL2/SDL_events.h>
#endif

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#ifdef ATLAS_USE_VULKAN
	#include "../vulkan/VKDevice.h"
#endif
//Atlas::NullGameWindow::operator WindowDescription() const {
//	return getWindowDescription();
//}



#ifdef ATLAS_USE_GLFW3

Atlas::GLFWGameWindow::GLFWGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings)
	: NullGameWindow(title, width, height, windowConfigFlags, targetFPS, icon), mGameWindowSettings(gameWindowSettings)
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

#endif

void Atlas::SDLGameWindow::init(const uint32_t cInitFlags)
{
	if (SDL_Init(cInitFlags) != 0) {
		throw std::runtime_error("Failed to initialize SDL");
	}

	mIsInitialized = true;
}

void Atlas::SDLGameWindow::update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {

			std::cout << "Window closed" << std::endl;
			mShouldClose = true;
		}

		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
				//event= true;
				std::cout << "Window minimized" << std::endl;
			}
			if (event.window.event == SDL_WINDOWEVENT_RESTORED) {
				//stop_rendering = false;

				std::cout << "Window restored" << std::endl;
			}
		}

		//send SDL event to imgui for handling
		ImGui_ImplSDL2_ProcessEvent(&event);
	}


}

void Atlas::SDLGameWindow::open(const uint32_t cOpenFlags)
{
	mSDLWindowPointer = SDL_CreateWindow(
		mWindowTitle.c_str(),
		mWindowRect.x,
		mWindowRect.y,
		mWindowRect.width,
		mWindowRect.height,
		cOpenFlags
	);

	if (this->mSDLWindowPointer == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}
	else {
		mIsOpen = true;
	}
}

bool Atlas::SDLGameWindow::shouldClose()
{
	return mShouldClose;
}

void Atlas::SDLGameWindow::close()
{
	if (mSDLWindowPointer != nullptr) {
		SDL_DestroyWindow(mSDLWindowPointer);
	}
}

void Atlas::SDLGameWindow::cleanup()
{
	close();

	SDL_Quit();
}

bool Atlas::SDLGameWindow::isOpen() const
{
	return mIsOpen;
}

bool Atlas::SDLGameWindow::isInitialized() const
{
	return mIsInitialized;
}

void Atlas::SDLGameWindow::setHint(WindowHint const& hint)
{
	SDL_SetHint(hint.identifier.c_str(), hint.value.c_str());
}

void Atlas::SDLGameWindow::setIcon(std::string const& newIconPath)
{
	mIconPath = newIconPath;
}

void Atlas::SDLGameWindow::setWindowTitle(std::string const& newWindowTitle)
{
	mWindowTitle = newWindowTitle;
}

void Atlas::SDLGameWindow::setWindowPosition(int newX, int newY)
{
	mWindowRect.x = newX;
	mWindowRect.y = newY;
}

void Atlas::SDLGameWindow::setWindowSize(int16_t newWidth, int16_t newHeight)
{
	mWindowRect.width = newWidth;
	mWindowRect.height = newHeight;
}

void Atlas::SDLGameWindow::setTargetFPS(int16_t newTargetFPS)
{
	mTargetFPS = newTargetFPS;
}
