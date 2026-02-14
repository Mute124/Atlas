#include "RenderPassesManager.h"

void Atlas::RenderPassesManager::addRenderPass(std::shared_ptr<RenderPass> renderPass) {
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

void Atlas::RenderPassesManager::beginDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	std::unique_lock lock(mRenderPassesMutex);

	for (auto& renderPass : mRenderPasses) {
		if (!renderPass->isValid())
		{
			continue;
		}

		renderPass->beginRenderPass(cmd, cDrawData);
	}
}

void Atlas::RenderPassesManager::drawRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	std::unique_lock lock(mRenderPassesMutex);

	for (auto& renderPass : mRenderPasses) {
		if (!renderPass->isValid())
		{
			continue;
		}

		renderPass->draw(cmd, cDrawData);

	}
}

void Atlas::RenderPassesManager::endDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	std::unique_lock lock(mRenderPassesMutex);

	for (auto& renderPass : mRenderPasses) {
		if (!renderPass->isValid())
		{
			continue;
		}

		renderPass->endRenderPass(cmd, cDrawData);

	}
}
