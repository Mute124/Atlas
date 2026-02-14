#include "PipelineLayout.h"

void Atlas::PipelineLayout::createPipelineLayout(VkDevice device, LayoutCreateInfo const& layoutInfo) {
	vkCreatePipelineLayout(device, &layoutInfo, nullptr, getHandlePtr());
}

Atlas::PipelineLayout::PipelineLayout(VkPipelineLayout handle) : AVulkanHandleWrapper<VkPipelineLayout>(handle) {}

Atlas::PipelineLayout::PipelineLayout(Device device, LayoutCreateInfo layoutInfo) {
	createPipelineLayout(device, layoutInfo);
}
