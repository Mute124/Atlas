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

	};
}
