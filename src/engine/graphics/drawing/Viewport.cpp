#include "Viewport.h"

#include <vulkan/vulkan_core.h>

void Atlas::Viewport::SetCommandBufferViewport(VkCommandBuffer commandBuffer, VkViewport const* viewport) {
	vkCmdSetViewport(commandBuffer, 0, 1, viewport);
}

Atlas::Viewport::Viewport(const VkViewport& mViewport)
	: mViewport(mViewport)
{
}

void Atlas::Viewport::set(VkCommandBuffer commandBuffer) {

	if (commandBuffer == VK_NULL_HANDLE) {
		return;
	}

	SetCommandBufferViewport(commandBuffer, &mViewport);
}

VkViewport Atlas::Viewport::get() const {
	return mViewport;
}
