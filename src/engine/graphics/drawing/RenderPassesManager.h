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

		void addRenderPass(std::shared_ptr<RenderPass> renderPass) {
			if (renderPass == nullptr || !renderPass->isValid())
			{
				ErrorLog(std::format("RenderPass is not valid: {}", renderPass->getName()));

				return;
			}

			std::unique_lock lock(mRenderPassesMutex);

			const RenderPassIndex index = mRenderPasses.size();
			renderPass->setIndex(index);

			mRenderPasses.push_back(renderPass);

		}

		void beginDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);

			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{
					continue;
				}

				renderPass->beginRenderPass(cmd, cDrawData);
			}
		}

		void drawRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);

			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{
					continue;
				}

				renderPass->draw(cmd, cDrawData);

			}
		}

		void endDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);

			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{
					continue;
				}

				renderPass->endRenderPass(cmd, cDrawData);

			}
		}
	};
}
