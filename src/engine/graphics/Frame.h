#pragma once

#include <memory>
#include <chrono>

#include "../core/Core.h"
#include "../core/Common.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	
	#include "vulkan/DeletionQueue.h"
#endif


namespace Atlas {
	using FrameCount = uint32_t;

#ifdef ATLAS_USE_VULKAN

	struct FrameData {
		VkCommandPool commandPool{ VK_NULL_HANDLE };
		VkCommandBuffer mainCommandBuffer{ VK_NULL_HANDLE };
		
		// Cant this be put in the swapchain class?
		VkSemaphore swapchainSemaphore{ VK_NULL_HANDLE };
		VkSemaphore renderSemaphore{ VK_NULL_HANDLE };

		VkFence renderFence{ VK_NULL_HANDLE };

		DeletionQueue deletionQueue;
	};

	constexpr unsigned int FRAME_OVERLAP = 2;
#endif

	class Frame {
	protected:
		void updateFrameCount();

	public:
		Counter<FrameCount> frameCounter;


		void update();

		FrameCount getFrameCount() const;
	};
}
