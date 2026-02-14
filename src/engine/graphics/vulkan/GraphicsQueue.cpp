#include "GraphicsQueue.h"

Atlas::GraphicsQueue::GraphicsQueue(VkQueue queue, uint32_t queueFamily)
	: mQueue(queue), mQueueFamily(queueFamily)
{
}

void Atlas::GraphicsQueue::submit(std::span<VkSubmitInfo2> submitInfos, VkFence fence)
{
	ATLAS_ASSERT(submitInfos.size() > 0, "No submit infos provided!");

	uint32_t submitInfoCount = (uint32_t)submitInfos.size();

	vkQueueSubmit2(mQueue, submitInfoCount, submitInfos.data(), fence);
}

void Atlas::GraphicsQueue::submit(VkSubmitInfo2 const& submitInfo, VkFence fence) {
	submit({ submitInfo }, fence);
}

VkQueue& Atlas::GraphicsQueue::getQueue() {
	return mQueue;
}

Atlas::GraphicsQueue::operator VkQueue& () {
	return const_cast<Atlas::GraphicsQueue&>(*this).getQueue();
}
