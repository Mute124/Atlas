#pragma once
#include <span>
#include <cstdint>

#include "../RenderCommon.h"

namespace Atlas {
	class GraphicsQueue {
	private:
		VkQueue mQueue{ VK_NULL_HANDLE };
		uint32_t mQueueFamily{ 1 };

	public:
		GraphicsQueue(VkQueue queue, uint32_t queueFamily)
			: mQueue(queue), mQueueFamily(queueFamily)
		{
		}

		GraphicsQueue() = default;

		void submit(std::span<VkSubmitInfo2> submitInfos, VkFence fence = VK_NULL_HANDLE)
		{
			ATLAS_ASSERT(submitInfos.size() > 0, "No submit infos provided!");

			uint32_t submitInfoCount = (uint32_t)submitInfos.size();

			vkQueueSubmit2(mQueue, submitInfoCount, submitInfos.data(), fence);
		}

		void submit(VkSubmitInfo2 const& submitInfo, VkFence fence = VK_NULL_HANDLE) {
			submit({ submitInfo }, fence);
		}

		VkQueue& getQueue() {
			return mQueue;
		}

		ATLAS_IMPLICIT operator VkQueue& () {
			const_cast<Atlas::GraphicsQueue&>(*this).getQueue();
		}
	};
}
