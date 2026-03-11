#pragma once

#include <glm/fwd.hpp>

#include  "../../core/Common.h"
#include "AllocatedImage.h"

namespace Atlas {



	class Swapchain : public InitializableAndValidatable {
	private:
		glm::vec2 mExtent;
		AllocatedImage mSwapchainImage;
	public:
		Swapchain(glm::vec2 extent) : mExtent(extent) {}
	};

}

