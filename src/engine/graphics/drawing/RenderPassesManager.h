#pragma once
#include <vector>
#include <memory>
#include <shared_mutex>
#include "RenderPass.h"

namespace Atlas {
	class RenderPassesManager {
	private:
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
		std::shared_mutex mRenderPassesMutex;

	public:

		void addRenderPass(std::shared_ptr<RenderPass> renderPass);

		void beginDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		void drawRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		void endDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);
	};
}
