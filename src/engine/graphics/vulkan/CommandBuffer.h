#pragma once

#include "../RenderCommon.h"
#include "../GraphicsUtils.h"
#include "PhysicalDevice.h"

namespace Atlas {
	class CommandBuffer : public AVulkanHandleWrapper<VkCommandBuffer> {
	public:
		using AllocateInfo = VkCommandBufferAllocateInfo;
		using BeginInfo = VkCommandBufferBeginInfo;
		using EResetFlag = VkCommandBufferResetFlags;

		ATLAS_IMPLICIT CommandBuffer(VkCommandBuffer handle) : AVulkanHandleWrapper<VkCommandBuffer>(handle) {}

		void allocate(Device const& device, AllocateInfo const* allocateInfo) {
			vkAllocateCommandBuffers(device.getHandle(), allocateInfo, getHandlePtr());
		}

		void allocate(Device const& device, VkCommandPool pool) {
			VkCommandBufferAllocateInfo info = CreateCommandBufferAllocateInfo(pool, 1);

			allocate(device, &info);
		}

		void begin(BeginInfo const& createInfo) {
			vkBeginCommandBuffer(getHandle(), &createInfo);
		}

		void end() {
			vkEndCommandBuffer(getHandle());
		}

		void reset(EResetFlag flags = 0) {
			vkResetCommandBuffer(getHandle(), flags);
		}

	};
}
