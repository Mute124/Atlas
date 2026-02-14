#include "CommandBuffer.h"

Atlas::CommandBuffer::CommandBuffer(VkCommandBuffer handle) : AVulkanHandleWrapper<VkCommandBuffer>(handle) {}

void Atlas::CommandBuffer::allocate(Device const& device, AllocateInfo const* allocateInfo) {
	vkAllocateCommandBuffers(device.getHandle(), allocateInfo, getHandlePtr());
}

void Atlas::CommandBuffer::allocate(Device const& device, VkCommandPool pool) {
	VkCommandBufferAllocateInfo info = CreateCommandBufferAllocateInfo(pool, 1);

	allocate(device, &info);
}

void Atlas::CommandBuffer::begin(BeginInfo const& createInfo) {
	vkBeginCommandBuffer(getHandle(), &createInfo);
}

void Atlas::CommandBuffer::end() {
	vkEndCommandBuffer(getHandle());
}

void Atlas::CommandBuffer::reset(EResetFlag flags) {
	vkResetCommandBuffer(getHandle(), flags);
}
