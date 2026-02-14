#pragma once

#include "../RenderCommon.h"
#include "../vulkan/PhysicalDevice.h"

namespace Atlas {
	class PipelineLayout : public AVulkanHandleWrapper<VkPipelineLayout> {
		friend class Pipeline;
	public:
		using LayoutCreateInfo = VkPipelineLayoutCreateInfo;

		//struct LayoutCreateInfo final : VkPipelineLayoutCreateInfo {
		//	using VkPipelineLayoutCreateInfo::VkPipelineLayoutCreateInfo;

		//	LayoutCreateInfo() : VkPipelineLayoutCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, } {}
		//};
	private:

		//VkPipelineLayout mLayout = VK_NULL_HANDLE;
		//LayoutCreateInfo mLayoutInfo{};

	protected:

		void createPipelineLayout(VkDevice device, LayoutCreateInfo const& layoutInfo) {
			vkCreatePipelineLayout(device, &layoutInfo, nullptr, getHandlePtr());
		}

	public:
		PipelineLayout(VkPipelineLayout handle) : AVulkanHandleWrapper<VkPipelineLayout>(handle) {}

		PipelineLayout(Device device, LayoutCreateInfo layoutInfo) {
			createPipelineLayout(device, layoutInfo);
		}

		PipelineLayout() = default;
	};
}
