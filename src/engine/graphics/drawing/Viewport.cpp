#include "Viewport.h"

#include <cmath>

#include <vulkan/vulkan_core.h>

#include <glm/fwd.hpp>

#include "..\..\debugging\AException.h"

void Atlas::Viewport::SetCommandBufferViewport(VkCommandBuffer commandBuffer, VkViewport const* viewport) {


	vkCmdSetViewport(commandBuffer, 0, 1, viewport);
}


Atlas::Viewport::Viewport(const VkViewport& mViewport)
	: mViewport(mViewport)
{
}

Atlas::Viewport::Viewport(glm::vec2 const& position, glm::vec2 const& size, float minDepth, float maxDepth)
	: mViewport({ position.x, position.y, size.x, size.y, minDepth, maxDepth })
{
}

void Atlas::Viewport::applyViewportToCommandBuffer(VkCommandBuffer commandBuffer) {
	if(commandBuffer == VK_NULL_HANDLE) {
		throw AException("Cannot apply viewport because the passed commandBuffer is VK_NULL_HANDLE!");
	}

	vkCmdSetViewport(commandBuffer, 0, 1, &mViewport);
}

void Atlas::Viewport::setPosition(glm::vec2 const& position)
{
	setPosition(position.x, position.y);
}

void Atlas::Viewport::setPosition(float x, float y) noexcept
{
	mViewport.x = x;
	mViewport.y = y;
}

void Atlas::Viewport::setSize(glm::vec2 const& size)
{
	setSize(size.x, size.y);
}

void Atlas::Viewport::setSize(float width, float height) noexcept
{
	
	mViewport.width = std::abs(width);
	mViewport.height = std::abs(height);
}

void Atlas::Viewport::setDepthRange(float minDepth, float maxDepth) noexcept
{
	mViewport.minDepth = minDepth;
	mViewport.maxDepth = maxDepth;
}

glm::vec2 Atlas::Viewport::getPosition() const {
	return {mViewport.x, mViewport.y};
}

glm::vec2 Atlas::Viewport::getSize() const { 
	return {mViewport.width, mViewport.height};
}

float Atlas::Viewport::getMinDepth() const noexcept {
	return mViewport.minDepth;
}

float Atlas::Viewport::getMaxDepth() const noexcept {
	return mViewport.maxDepth;
}

VkViewport Atlas::Viewport::getViewport() const noexcept {
	return mViewport;
}

