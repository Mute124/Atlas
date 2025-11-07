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

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace Atlas {
	struct ComputePushConstants {
		glm::vec4 data1;
		glm::vec4 data2;
		glm::vec4 data3;
		glm::vec4 data4;
	};

	struct ComputeEffect {
		const char* name;

		VkPipeline pipeline;
		VkPipelineLayout layout;

		ComputePushConstants data;
	};
}
