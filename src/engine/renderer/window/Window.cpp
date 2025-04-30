#include <stdexcept>
#include <unordered_map>
#include <string>

#include "Window.h"

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>
#elif defined(ATLAS_USE_SDL2)

	#include <SDL2/SDL.h>
#endif

Atlas::IGameWindow::IGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon)
	: mWindowTitle(title), mWindowWidth(width), mWindowHeight(height), mWindowConfigFlags(windowConfigFlags), mTargetFPS(targetFPS), mWindowIcon(icon) {
}

#ifdef ATLAS_USE_GLFW3

Atlas::GLFWGameWindow::GLFWGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfigFlags, unsigned int targetFPS, std::string const& icon, GameWindowSettings const& gameWindowSettings)
	: IGameWindow(title, width, height, windowConfigFlags, targetFPS, icon), mGameWindowSettings(gameWindowSettings)
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
: IGameWindow(title, width, height, windowConfigFlags, targetFPS, icon), mGameWindowSettings(gameWindowSettings)
{
}

Atlas::SDLGameWindow::~SDLGameWindow()
{
}

void Atlas::SDLGameWindow::init()
{
}

void Atlas::SDLGameWindow::open()
{
}

void Atlas::SDLGameWindow::update()
{
}

bool Atlas::SDLGameWindow::shouldClose()
{
	return false;
}

void Atlas::SDLGameWindow::close(bool shouldCleanup)
{
}

void Atlas::SDLGameWindow::cleanup()
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