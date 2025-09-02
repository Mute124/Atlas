
#include "Frame.h"
#include "backend/VKDevice.h"


void Atlas::Frame::init(std::unique_ptr<VkQueue> graphicsQueue, std::unique_ptr<VkDevice> device, std::unique_ptr<VkCommandPoolCreateInfo> createInfo)
{
	vkCreateCommandPool(*device, createInfo.get(), nullptr, &commandPool);

	// allocate the default command buffer that we will use for rendering
	VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(commandPool, 1);

	vkAllocateCommandBuffers(*device, &cmdAllocInfo, &mainCommandBuffer);
}

void Atlas::Frame::shutdown(std::unique_ptr<VkDevice> device)
{
	vkDestroyCommandPool(*device, commandPool, nullptr);
}
