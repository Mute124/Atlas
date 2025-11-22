/**************************************************************************************************
 * @file Effect.h
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Atlas {
	struct ComputePushConstants {
		glm::vec4 data1;
		glm::vec4 data2;
		glm::vec4 data3;
		glm::vec4 data4;
	};

	class ComputeEffect {
		friend class EffectManager;
	public:
		const char* name;
		int index;

		VkPipeline pipeline;
		VkPipelineLayout layout;

		ComputePushConstants data;
		
		ComputeEffect() = default;

		void bind(VkCommandBuffer commandBuffer);
	};
}
