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

		ATLAS_IMPLICIT CommandBuffer(VkCommandBuffer handle);

		void allocate(Device const& device, AllocateInfo const* allocateInfo);

		void allocate(Device const& device, VkCommandPool pool);

		void begin(BeginInfo const& createInfo);

		void end();

		void reset(EResetFlag flags = 0);

	};
}
