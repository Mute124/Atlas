#pragma once

#include "RenderingBackend.h"

namespace Atlas {
#ifdef ATLAS_USE_VULKAN
		class VulkanRenderingBackend : public IRenderingBackend {
		public:
			
			void init() override;

			void update() override;

			void shutdown() override;
		};
#endif
}