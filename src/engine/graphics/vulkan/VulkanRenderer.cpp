#include "VulkanRenderer.h"
#include "../../debugging/Logging.h"
#include "../../debugging/AException.h"

void Atlas::VulkanRenderer::init(std::unique_ptr<GameWindow> window) {
	if (window == nullptr || !window->isInit()) {
		throw AException("Window not initialized, and must be initialized before the renderer can be initialized.");
	}


}
