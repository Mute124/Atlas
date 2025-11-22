/**************************************************************************************************
 * @file DescriptorLayoutBuilder.cpp
 * 
 * @brief The definition of DescriptorLayoutBuilder.
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <cstdint>

#include <vulkan/vulkan.h>

#include "DescriptorLayoutBuilder.h"

void Atlas::DescriptorLayoutBuilder::addBinding(uint32_t binding, VkDescriptorType type)
{
	VkDescriptorSetLayoutBinding newbind{};
	newbind.binding = binding;
	newbind.descriptorCount = 1;
	newbind.descriptorType = type;

	bindings.push_back(newbind);
}

void Atlas::DescriptorLayoutBuilder::clear()
{
	bindings.clear();
}

VkDescriptorSetLayout Atlas::DescriptorLayoutBuilder::build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext, VkDescriptorSetLayoutCreateFlags flags)
{
	for (auto& b : bindings) {
		b.stageFlags |= shaderStages;
	}

	VkDescriptorSetLayoutCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	info.pNext = pNext;

	info.pBindings = bindings.data();
	info.bindingCount = (uint32_t)bindings.size();
	info.flags = flags;

	VkDescriptorSetLayout set;
	vkCreateDescriptorSetLayout(device, &info, nullptr, &set);

	return set;
}
