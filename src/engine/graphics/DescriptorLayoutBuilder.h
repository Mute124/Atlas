/**************************************************************************************************
 * @file DescriptorLayoutBuilder.h
 * 
 * @brief Contains the DescriptorLayoutBuilder class declaration. This file is part of Atlas' Vulkan graphics system.
 * 
 * @date September 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <vector>
#include <cstdint>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Atlas {
	class DescriptorLayoutBuilder {
	public:
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		void addBinding(uint32_t binding, VkDescriptorType type);

		void clear();

		VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
	};
}
