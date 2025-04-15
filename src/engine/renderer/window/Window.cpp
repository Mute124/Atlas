#include "Window.h"

#ifdef ATLAS_USE_RAYLIB
	#include <raylib.h>
#endif


#include "WindowDecorations.h"
#include <string>

Atlas::IGameWindow::IGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfig, unsigned int targetFPS, std::string const& icon)
	: mWindowTitle(title), mWindowWidth(width), mWindowHeight(height), mWindowConfigFlags(windowConfig), mTargetFPS(targetFPS), mWindowIcon(icon) {
}

#ifdef ATLAS_USE_GLFW3

Atlas::GLFWGameWindow::~GLFWGameWindow() { 
	close(true); 
}

void Atlas::GLFWGameWindow::init()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	
}

void Atlas::GLFWGameWindow::open()
{
	this->mGLFWWindowPointer = glfwCreateWindow(this->mWindowWidth, this->mWindowHeight, this->mWindowTitle.c_str(), nullptr, nullptr);
}

void Atlas::GLFWGameWindow::update()
{
	glfwPollEvents();
}

bool Atlas::GLFWGameWindow::shouldClose()
{
	return glfwWindowShouldClose(this->mGLFWWindowPointer);
}

void Atlas::GLFWGameWindow::close(bool shouldCleanup)
{
	glfwDestroyWindow(this->mGLFWWindowPointer);
	
	// in order to simplify the deinitialization process, this function may also call cleanup.
	if (shouldCleanup) {
		this->cleanup();
	}
}

void Atlas::GLFWGameWindow::cleanup()
{
	glfwTerminate();
}

#endif // ATLAS_USE_GLFW3


