#include "Renderer.h"

#include "backend/RenderingBackend.h"
#include "window/Window.h"
#include "../core/Core.h"

Atlas::ARenderer::ARenderer(AGameWindow* gameWindow, ARenderingBackend* backend, bool canBeMultiThreaded)
	: mainGameWindow(gameWindow), renderingBackend(backend), mCanBeMultiThreaded(canBeMultiThreaded)
{
}

Atlas::ARenderer::ARenderer(AGameWindow* gameWindow, ARenderingBackend* backend)
	: mainGameWindow(gameWindow), renderingBackend(backend)
{
}

bool Atlas::ARenderer::canBeMultiThreaded() const {
	return this->mCanBeMultiThreaded; 
}

bool Atlas::ARenderer::isInitialized() const {
	return this->mIsInitialized; 
}

