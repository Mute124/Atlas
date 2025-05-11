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



Atlas::NullGameWindow::NullGameWindow(WindowDescription const& windowDescription, const uint32_t cGraphicsAPIFlag)
	: IGameWindow(), mWindowDescription(windowDescription)
{
	if (sHasSetGraphicsAPIFlag == false) {
		sGraphicsAPIFlag = cGraphicsAPIFlag;
		sHasSetGraphicsAPIFlag = true;
	}
}



Atlas::NullGameWindow::NullGameWindow(WindowDescription const& windowDescription)
	: NullGameWindow(windowDescription, sGraphicsAPIFlag)
{
}

void Atlas::NullGameWindow::init()
{
	this->getWindowDescription().isInitialized = true;
}

void Atlas::NullGameWindow::open()
{
	this->getWindowDescription().isOpen = true;
}

void Atlas::NullGameWindow::close()
{
	this->getWindowDescription().isOpen = false;
}

bool Atlas::NullGameWindow::isOpen() const
{
	return this->mWindowDescription.isOpen;
}

bool Atlas::NullGameWindow::isInitialized() const
{
	return this->mWindowDescription.isInitialized;
}

void Atlas::NullGameWindow::setFlag(uint32_t flagIdentifier, uint32_t newValue)
{
	this->getWindowDescription().windowConfigFlags[flagIdentifier] = newValue;
}

void Atlas::NullGameWindow::setFlag(uint32_t flagIdentifier, std::string const& newValue)
{
	this->getWindowDescription().windowConfigFlags[flagIdentifier] = newValue;
}

void Atlas::NullGameWindow::setIcon(std::string const& newIconPath)
{
	this->getWindowDescription().iconPath = newIconPath;
}

Atlas::WindowDescription& Atlas::NullGameWindow::getWindowDescription() noexcept
{
	return this->mWindowDescription;
}

uint32_t Atlas::NullGameWindow::getGraphicsAPIFlag() const noexcept
{
	return sGraphicsAPIFlag;
}

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

uint32_t Atlas::SDLGameWindow::GetGraphicsAPIFlag()
{
	uint32_t result = 0;

#ifdef ATLAS_USE_OPENGL
	result = SDL_WINDOW_OPENGL;
#elif defined(ATLAS_USE_VULKAN)
	result = SDL_WINDOW_VULKAN;

#elif defined(ATLAS_USE_METAL)

	result = SDL_WINDOW_METAL;

#else

	ATLAS_STATIC_ASSERT(false, "Unknown graphics API! Please define ATLAS_USE_OPENGL, ATLAS_USE_VULKAN or ATLAS_USE_METAL!");
#endif

	return result;
}

Atlas::SDLGameWindow::SDLGameWindow(WindowDescription const& windowDescription, const uint32_t cGraphicsAPIFlag)
	: NullGameWindow(windowDescription, cGraphicsAPIFlag)
{
}

Atlas::SDLGameWindow::SDLGameWindow(WindowDescription const& windowDescription)
	: SDLGameWindow(windowDescription, GetGraphicsAPIFlag())
{
}

bool Atlas::SDLGameWindow::EventOccurred()
{
	// Since the documentation of SDL_PollEvent states that this function will return whether or not there is a 
	// pending event in the event queue if the parameter is null, we can safely ignore the return value here.
	return SDL_PollEvent(nullptr) != 0;
}

void Atlas::SDLGameWindow::setFlag(uint32_t flagIdentifier, uint32_t newValue)
{
	NullGameWindow::setFlag(flagIdentifier, newValue);

}

void Atlas::SDLGameWindow::setFlag(uint32_t flagIdentifier, std::string const& newValue)
{
	NullGameWindow::setFlag(flagIdentifier, newValue);

}

void Atlas::SDLGameWindow::setIcon(std::string const& newIconPath)
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
		this->getWindowDescription().isInitialized = true;
	}

	this->mEventHandlers[SDL_QUIT] = [](SDL_Event const& event, SDLGameWindow& window) { 
		window.close();
		return 0;
	};

}

void Atlas::SDLGameWindow::open()
{
	WindowDescription const& windowDescription = this->getWindowDescription();

	this->mSDLWindowPointer = SDL_CreateWindow(
		windowDescription.windowTitleString.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowDescription.windowRectangle.width,
		windowDescription.windowRectangle.height,
		(SDL_WindowFlags)(SDL_WINDOW_VULKAN)
	);
	
	if (this->mSDLWindowPointer == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}
	else {
		// this parent function actually only sets the value of windowDescription.isOpen.
		NullGameWindow::open();
	}

	SDL_SetWindowPosition(this->mSDLWindowPointer, windowDescription.windowRectangle.x, windowDescription.windowRectangle.y);
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

	while(SDL_PollEvent(&sdlEvent)) {

		if(sdlEvent.type == SDL_QUIT) {
			this->mShouldClose = true;
		}

		//eventsExist = EventOccurred();
	}

}

bool Atlas::SDLGameWindow::shouldClose()
{
	return this->mShouldClose;
}

void Atlas::SDLGameWindow::close()
{
	this->setFlag(SDL_WINDOW_HIDDEN, SDL_TRUE);
}

void Atlas::SDLGameWindow::cleanup()
{
	if (this->mSDLWindowPointer != nullptr) {
		SDL_DestroyWindow(this->mSDLWindowPointer);
	}

	SDL_Quit();
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



