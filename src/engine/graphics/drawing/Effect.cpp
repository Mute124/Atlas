/**************************************************************************************************
 * @file Effect.cpp
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <string>

#include <vulkan/vulkan.h>

#include "Effect.h"

void Atlas::ComputeEffect::bind(VkCommandBuffer commandBuffer)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
}
