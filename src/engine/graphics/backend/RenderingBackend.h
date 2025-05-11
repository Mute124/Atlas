/**************************************************************************************************
 * @file RenderingBackend.h
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <functional>
#include <cstdint>
#include <vector>

#include "../../core/Core.h"

#include "../../debugging/Debugger.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif


namespace Atlas {

	enum class TextureFormat { RGBA8, Depth32 };

	struct RenderTargetDesc { 
		int width;
		int height;
		TextureFormat format; 
	};

	class RenderTarget {
	public:
		RenderTarget(RenderTargetDesc d) : desc(d) {}
		int getWidth() const { return desc.width; }
		int getHeight() const { return desc.height; }
		TextureFormat getFormat() const { return desc.format; }
	private:
		RenderTargetDesc desc;
	};

	struct PipelineDesc { std::string vertPath, fragPath; };

	class Pipeline {
	public:
		Pipeline(PipelineDesc d) : desc(d) {}
	private:
		PipelineDesc desc;
	};

	// RenderPass describes a single pass in the graph
	struct RenderPass {
		std::string name;
		std::shared_ptr<Pipeline> pipeline;
		std::vector<std::shared_ptr<RenderTarget>> inputs;
		std::shared_ptr<RenderTarget> output;
		std::function<void()> onExecute;
	};

	// RenderGraph holds passes and executes them (sequentially by default)
	class RenderGraph {
	public:
		auto createRenderTarget(RenderTargetDesc d) {
			return std::make_shared<RenderTarget>(d);
		}
		auto createPipeline(PipelineDesc d) {
			return std::make_shared<Pipeline>(d);
		}
		void addPass(const RenderPass& p) { passes.push_back(p); }
		const std::vector<RenderPass>& getPasses() const { return passes; }
		void execute() {
			for (auto& p : passes) {
				std::cout << "[RenderGraph] Executing: " << p.name << std::endl;
				if (p.onExecute) p.onExecute();
			}
		}
	private:
		std::vector<RenderPass> passes;
	};

	class IRenderingBackend {
	public:

		virtual void init() = 0;

		virtual void update() = 0;
		
		virtual void shutdown() = 0;
	};

#ifdef ATLAS_USE_VULKAN
	
	class VulkanRenderingBackend : public IRenderingBackend {
	private:

		VkInstance mVulkanInstance = VK_NULL_HANDLE;
		std::vector<const char*> mValidationLayers;

	protected:

		VkApplicationInfo mAppInfo;
		VkInstanceCreateInfo mCreateInfo;

	public:

		VulkanRenderingBackend(const VulkanRenderingBackend&) = delete;

		/**
		 * @brief Default constructor.
		 *
		 * @note This constructor is not intended to be used because the @ref mAppInfo and @ref mCreateInfo are
		 * required to create a Vulkan instance (and they are declared as a const!). Additionally, the variable
		 * @ref mCreateInfo requires information on extensions and such, and since Atlas does not mandate any
		 * specific window management system (ie. GLFW), the @ref mAppInfo @b MUST be set prior to calling this.
		 *
		 * @since v0.0.1
		 *
		 * @sa @ref mAppInfo
		 * @sa @ref mCreateInfo
		 */
		VulkanRenderingBackend() = default;

		void init() override;

		void update() override;

		void shutdown() override;

		bool checkValidationLayerSupport();
	};
#endif
}