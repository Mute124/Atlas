#include "Renderer.h"

#include "backend/RenderingBackend.h"
#include "window/Window.h"
#include "../core/Core.h"

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, RenderingBackend* backend, bool canBeMultiThreaded)
	: mainGameWindow(gameWindow), renderingBackend(backend), mCanBeMultiThreaded(canBeMultiThreaded)
{
}

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, RenderingBackend* backend)
	: mainGameWindow(gameWindow), renderingBackend(backend)
{
}

bool Atlas::IRenderer::canBeMultiThreaded() const {
	return this->mCanBeMultiThreaded; 
}

bool Atlas::IRenderer::isInitialized() const {
	return this->mIsInitialized; 
}

#ifdef ATLAS_USE_VULKAN

Atlas::VulkanRenderer::VulkanRenderer(IGameWindow* gameWindow, VulkanRenderingBackend* backend)
	: IRenderer(gameWindow, backend, true)
{
}

Atlas::VulkanRenderer::VulkanRenderer(IGameWindow* gameWindow)
	: VulkanRenderer(gameWindow, new VulkanRenderingBackend())
{
}

void Atlas::VulkanRenderer::init()
{
	// TODO: make this not hardcoded
	this->mainGameWindow->init(SDL_INIT_VIDEO);
	
	this->renderingBackend->init();

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

			this->renderingBackend->update();
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