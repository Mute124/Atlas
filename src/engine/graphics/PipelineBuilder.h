/**************************************************************************************************
 * @file PipelineBuilder.h
 * 
 * @brief This file contains the declaration of the @ref PipelineBuilder class and it's associated declarations.
 * This file is part of Atlas' Vulkan graphics system.
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <vector>

#include <vulkan/vulkan.h>

#include "GraphicsUtils.h"

namespace Atlas {
	struct PipelineBuilderSettings {
		VkPipelineInputAssemblyStateCreateInfo inputAssembly;
		VkPipelineRasterizationStateCreateInfo rasterizer;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo multisampling;
		VkPipelineLayout pipelineLayout;
		VkPipelineDepthStencilStateCreateInfo depthStencil;
		VkPipelineRenderingCreateInfo renderInfo;
		VkFormat colorAttachmentformat;
	};

	class PipelineBuilder {
	public:
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		PipelineBuilderSettings settings;

		PipelineBuilder() { clear(); }

		void clear();

		VkPipeline buildPipeline(VkDevice device);

		void setShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader) {
			shaderStages.clear();
			
			shaderStages.push_back(
				CreatePipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));

			shaderStages.push_back(
				CreatePipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));
		}

		void setInputTopography(VkPrimitiveTopology topology) {
			settings.inputAssembly.topology = topology;
			// we are not going to use primitive restart on the entire tutorial so leave
			// it on false
			settings.inputAssembly.primitiveRestartEnable = VK_FALSE;
		}

		void setPolygonMode(VkPolygonMode mode) {
			settings.rasterizer.polygonMode = mode;
			settings.rasterizer.lineWidth = 1.f;
		}

		void setCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace)
		{
			settings.rasterizer.cullMode = cullMode;
			settings.rasterizer.frontFace = frontFace;
		}

		void setMultisamplingNone()
		{
			settings.multisampling.sampleShadingEnable = VK_FALSE;
			// multisampling defaulted to no multisampling (1 sample per pixel)
			settings.multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			settings.multisampling.minSampleShading = 1.0f;
			settings.multisampling.pSampleMask = nullptr;
			// no alpha to coverage either
			settings.multisampling.alphaToCoverageEnable = VK_FALSE;
			settings.multisampling.alphaToOneEnable = VK_FALSE;
		}

		void disableBlending()
		{
			// default write mask
			settings.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			// no blending
			settings.colorBlendAttachment.blendEnable = VK_FALSE;
		}

		void setColorAttachmentFormat(VkFormat format)
		{
			settings.colorAttachmentformat = format;
			// connect the format to the renderInfo  structure

			settings.renderInfo.colorAttachmentCount = 1;
			settings.renderInfo.pColorAttachmentFormats = &settings.colorAttachmentformat;
		}

		void setDepthFormat(VkFormat format)
		{
			settings.renderInfo.depthAttachmentFormat = format;
		}

		void disableDepthTest() {
			settings.depthStencil.depthTestEnable = VK_FALSE;
			settings.depthStencil.depthWriteEnable = VK_FALSE;
			settings.depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
			settings.depthStencil.depthBoundsTestEnable = VK_FALSE;
			settings.depthStencil.stencilTestEnable = VK_FALSE;
			settings.depthStencil.front = {};
			settings.depthStencil.back = {};
			settings.depthStencil.minDepthBounds = 0.f;
			settings.depthStencil.maxDepthBounds = 1.f;
		}
	};
}
