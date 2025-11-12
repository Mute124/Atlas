
#include "Frame.h"
#include "backend/VKDevice.h"


void Atlas::Frame::setFrameCount(FrameCount newFrameCount) {
	ATLAS_ASSERT(newFrameCount >= 0, "Frame count must be greater than or equal to 0.");
	ATLAS_ASSERT(newFrameCount <= ATLAS_UINT32_LIMIT, "Frame count must be less than or equal to UINT32_MAX.");

	mFrameCount = newFrameCount;
}

void Atlas::Frame::updateFrameCount() {
	// Using this method rather than simply incrementing the frame count through mFrameCount++
	// because the setFrameCount function has checks
	setFrameCount(mFrameCount + 1);
}

void Atlas::Frame::init(std::unique_ptr<VkQueue> graphicsQueue, std::unique_ptr<VkDevice> device, std::unique_ptr<VkCommandPoolCreateInfo> createInfo)
{
	//vkCreateCommandPool(*device, createInfo.get(), nullptr, &commandPool);

	//// allocate the default command buffer that we will use for rendering
	//VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(commandPool, 1);

	//vkAllocateCommandBuffers(*device, &cmdAllocInfo, &mainCommandBuffer);
}

void Atlas::Frame::shutdown(std::unique_ptr<VkDevice> device)
{
	//vkDestroyCommandPool(*device, commandPool, nullptr);
}

Atlas::FrameCount Atlas::Frame::getFrameCount() const {
	return mFrameCount;
}
