#pragma once

#include "../vulkan/CommandBuffer.h"
#include "../RenderCommon.h"
#include "PipelineLayout.h"
#include "Shader.h"
namespace Atlas {
	class Pipeline : public AVulkanHandleWrapper<VkPipeline> {
	public:
		enum class EBindPoint {
			Graphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
			Compute = VK_PIPELINE_BIND_POINT_COMPUTE,
			RayTracingKHR = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
			RayTracingNV = VK_PIPELINE_BIND_POINT_RAY_TRACING_NV,
			Max = VK_PIPELINE_BIND_POINT_MAX_ENUM
		};
	private:
		EBindPoint mBindPoint{ EBindPoint::Graphics };

	protected:
		//VkPipelineLayoutCreateInfo mComputeLayoutInfo;
		//VkPushConstantRange mPushConstantRange;

		//VkPipeline mPipeline{ VK_NULL_HANDLE };
		PipelineLayout mPipelineLayout;

		//VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		Shader mShader;

		void setBindPoint(EBindPoint newBindPoint);

	public:

		Pipeline(EBindPoint bindPoint, VkPipeline pipeline, PipelineLayout pipelineLayout);

		Pipeline() = default;

		void bind(CommandBuffer commandBuffer);

		EBindPoint getBindPoint() const noexcept;

		//void createLayout(VkPipelineLayoutCreateInfo layoutInfo, VkDevice* device)
		//{
		//	vkCreatePipelineLayout(*device, &layoutInfo, nullptr, &mPipelineLayout);
		//}
		//void createLayout(VkDevice* device, VkDescriptorSetLayout* descriptorSetLayout) {
		//	VkPipelineLayoutCreateInfo layoutInfo{};
		//	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		//	layoutInfo.pNext = nullptr;
		//	layoutInfo.pSetLayouts = descriptorSetLayout;
		//	layoutInfo.setLayoutCount = 1;
		//	createLayout(layoutInfo, device);
		//}
		//void init(VkDevice* device, DeletionQueue* deletionQueue, VkPipelineShaderStageCreateInfo stageInfo, VkComputePipelineCreateInfo pipelineInfo) {
		//	vkCreateComputePipelines(*device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline);
		//	vkDestroyShaderModule(*device, mShader.getModule(), nullptr)
		//	deletionQueue->push([&]() {
		//		destroyPipeline(device);
		//	});
		//}
		//void destroyPipeline(VkDevice* device) {
		//	vkDestroyPipelineLayout(*device, mPipelineLayout, nullptr);
		//	vkDestroyPipeline(*device, mPipeline, nullptr);
		//}
	};
}
