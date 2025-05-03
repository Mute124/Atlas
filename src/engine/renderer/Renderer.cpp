#include "Renderer.h"

#include "backend/RenderingBackend.h"
#include "window/Window.h"
#include "../core/Core.h"
#include "backend/VulkanRenderingBackend.h"

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, IRenderingBackend* backend, bool canBeMultiThreaded)
	: mainGameWindow(gameWindow), renderingBackend(backend), mCanBeMultiThreaded(canBeMultiThreaded)
{
}

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, IRenderingBackend* backend)
	: mainGameWindow(gameWindow), renderingBackend(backend)
{
}

bool Atlas::IRenderer::canBeMultiThreaded() const { return this->mCanBeMultiThreaded; }

bool Atlas::IRenderer::isInitialized() const { return this->mIsInitialized; }

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
	this->mainGameWindow->init();
	this->renderingBackend->init();
}

void Atlas::VulkanRenderer::update()
{
	while (!this->mainGameWindow->shouldClose()) {
		if (this->mainGameWindow != nullptr) {
			this->mainGameWindow->update();
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
	this->mainGameWindow->close(true);
}

#endif