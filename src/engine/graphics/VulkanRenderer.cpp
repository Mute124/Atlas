#include "VulkanRenderer.h"

#ifdef ATLAS_USE_VULKAN

Atlas::VulkanRenderer::VulkanRenderer(AGameWindow* gameWindow, VulkanRenderingBackend* backend)
	: ARenderer(gameWindow, backend, true)
{
}

Atlas::VulkanRenderer::VulkanRenderer(AGameWindow* gameWindow)
	: VulkanRenderer(gameWindow, nullptr)
{
}

void Atlas::VulkanRenderer::init()
{
	// TODO: make this not hardcoded
	this->mainGameWindow->init(SDL_INIT_VIDEO);

	//this->renderingBackend->init();

	this->mIsInitialized = true;
}

void Atlas::VulkanRenderer::update()
{
	while (!WindowShouldClose(this->mainGameWindow)) {

		if (this->mainGameWindow != nullptr) {

			this->mainGameWindow->update();

			// Check to see if there was a request to close the window that occured during the frame update
			if (WindowShouldClose(this->mainGameWindow)) {
				this->mainGameWindow->close();
				this->mainGameWindow->cleanup();

				break;
			}
		}
		else {
#ifdef ATLAS_DEBUG

			std::cout << "Main game window is null!" << std::endl;
#endif
		}

		if (this->renderingBackend != nullptr) {

			//this->renderingBackend->update();
		}
		else {
#ifdef ATLAS_DEBUG
			std::cout << "Rendering backend is null!" << std::endl;
#endif
		}
	}
}

void Atlas::VulkanRenderer::cleanup()
{
	if (this->mainGameWindow->isOpen()) {
		this->mainGameWindow->close();
		this->mainGameWindow->cleanup();
	}
}

bool Atlas::VulkanRenderer::shouldExit()
{
	return this->mainGameWindow->shouldClose();
}

#endif