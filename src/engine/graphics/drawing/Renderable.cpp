#include "Renderable.h"

void Atlas::Renderable::setName(std::string const& name) { mName = name; }

Atlas::Renderable::Renderable(std::string const& name) : mName(name) {}

Atlas::Renderable::Renderable() : Renderable("Unnamed Renderable") {}

void Atlas::Renderable::SetCurrentDrawData(CurrentDrawData const& currentFrame) {
	//std::scoped_lock lock(sCurrentDrawDataMutex);
	//sCurrentDrawData = currentFrame;
}

bool Atlas::Renderable::beginDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData, EffectManager& computeEffects) {
	return 0;
}

bool Atlas::Renderable::draw(VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	return 0;
}

bool Atlas::Renderable::endDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	return 0;
}

std::string_view Atlas::Renderable::getName() const { return mName; }
