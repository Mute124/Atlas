#pragma once

#include <memory>
#include <chrono>

#include "../core/Core.h"
#include "../core/Common.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif

#include "DeletionQueue.h"

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
	public:
		class FrameCounter : public Counter<FrameCount> {
		public:
		};

	private:
		FrameCount mFrameCount = 0;
		
	protected:
		void setFrameCount(FrameCount newFrameCount);

		void updateFrameCount();

	public:

//#ifdef ATLAS_USE_VULKAN
//		VkCommandPool commandPool = VK_NULL_HANDLE;
//		VkCommandBuffer mainCommandBuffer = VK_NULL_HANDLE;
//
//		VkSemaphore swapchainSemaphore = VK_NULL_HANDLE;
//		VkSemaphore renderSemaphore = VK_NULL_HANDLE;
//
//		VkFence renderFence = VK_NULL_HANDLE;
//#endif

		void init(std::unique_ptr<VkQueue> graphicsQueue, std::unique_ptr<VkDevice> device, std::unique_ptr<VkCommandPoolCreateInfo> createInfo);

		void shutdown(std::unique_ptr<VkDevice> device);

		FrameCount getFrameCount() const;
	};

	class RenderFrame : public Frame {
		friend class VulkanRenderer;
		friend class VulkanRenderingBackend;
	private:
		FrameData mFrameData;

	public:

	};
}
