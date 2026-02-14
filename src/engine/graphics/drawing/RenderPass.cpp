#include "Renderpass.h"

void Atlas::RenderPass::setIndex(RenderPassIndex index) {
	// TODO: check if index is valid
	mIndex = index;
}

void Atlas::RenderPass::beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	// Empty because not all render passes need things to do at the start
}

void Atlas::RenderPass::endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	// Empty because not all render passes need things to do at the end
}

Atlas::RenderPassIndex Atlas::RenderPass::getID() const {
	return mIndex;
}
