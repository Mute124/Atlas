#pragma once

#include <string>
#include <cstdint>


#include "../../core/Core.h" // this include has to be put here because the GLFW_INCLUDE_VULKAN is defined in this file (of course if vulkan and GLFW3 is being used!)

#ifdef ATLAS_USE_GLFW3
	#include <GLFW/glfw3.h>
#endif // ATLAS_USE_GLFW3


namespace Atlas {
	/**
	 * @brief Window decorations are a set of parameters that are used to configure the window. This includes title, width, height, window config, and target FPS.
	 */
	//class WindowDecorations {
	//public:
	//	const char* title = "Untitled Atlas Project";
	//	uint32_t width = 1280; /// <summary> The width of the window. </summary>
	//	uint32_t height = 720; /// <summary> The height of the window. </summary>
	//	unsigned int windowConfig = -1; /// <summary> For available config flags, see <see cref="ConfigFlags"/> in the raylib documentation.</summary>
	//	unsigned int targetFPS = 60; /// <summary> The target FPS for the window. </summary>
	//	const char* icon = "\\engine\\assets\\engine\\techstorm.png";
	//};

	class IGameWindow {
	protected:
		std::string mWindowTitle;
		std::string mWindowIcon;

		uint32_t mWindowWidth;
		uint32_t mWindowHeight;
		
		unsigned int mWindowConfigFlags;
		unsigned int mTargetFPS;

		bool mIsOpen;
		bool mIsInitialized;

	public:
		IGameWindow(std::string const& title, uint32_t width, uint32_t height, unsigned int windowConfig, unsigned int targetFPS, std::string const& icon);

		virtual void init() = 0;

		virtual void open() = 0;

		virtual void update() = 0;

		virtual bool shouldClose() = 0;

		virtual void close(bool shouldCleanup) = 0;

		virtual void cleanup() = 0;
	};

#ifdef ATLAS_USE_GLFW3
	class GLFWGameWindow : public IGameWindow {
	private:
		GLFWwindow* mGLFWWindowPointer = nullptr;
	public:
		using IGameWindow::IGameWindow;

		virtual ~GLFWGameWindow();

		// Inherited via IWindow
		void init() override;

		void open() override;

		void update() override;

		bool shouldClose() override;

		void close(bool shouldCleanup) override;

		void cleanup() override;
	};
#endif // ATLAS_USE_GLFW3
}
