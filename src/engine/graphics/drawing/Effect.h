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
	struct ComputePushConstant {
		glm::vec4 data;
		std::string name = "null";

	};

	struct ComputePushConstants {
		ComputePushConstant data1;
		ComputePushConstant data2;
		ComputePushConstant data3;
		ComputePushConstant data4;
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