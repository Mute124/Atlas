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
		GraphicsQueue(VkQueue queue, uint32_t queueFamily);

		GraphicsQueue() = default;

		void submit(std::span<VkSubmitInfo2> submitInfos, VkFence fence = VK_NULL_HANDLE);

		void submit(VkSubmitInfo2 const& submitInfo, VkFence fence = VK_NULL_HANDLE);

		VkQueue& getQueue();

		ATLAS_IMPLICIT operator VkQueue& ();
	};
}
