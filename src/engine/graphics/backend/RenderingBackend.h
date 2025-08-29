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

#include <optional>
#include <string>
#include <functional>
#include <cstdint>
#include <vector>
#include <initializer_list>


#include "../../core/Core.h"

#include "../../debugging/Debugger.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif


namespace Atlas {

	class IGameWindow;

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

	class RenderingBackend;

	// RenderGraph holds passes and executes them (sequentially by default)
	class RenderGraph {
	private:
		std::vector<RenderPass> passes;
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
	};


	class RenderingBackend {
	public:
		struct APIVersion {
			uint32_t major = -1;
			uint32_t minor = -1;
			uint32_t patch = -1;
		};

	private:
		APIVersion mAPIVersion;
		
		bool mbUseDebuggingTools = false;
		bool mbEnableErrorChecking = false; // dont worry about this if you are not using vulkan
	public:
		
		RenderingBackend() = default;

		explicit RenderingBackend(APIVersion version, const bool cbUseValidationLayers) : mAPIVersion(version), mbEnableErrorChecking(cbUseValidationLayers) {}

		virtual void setAPIVersion(uint32_t major, uint32_t minor, uint32_t patch) { this->mAPIVersion.major = major; this->mAPIVersion.minor = minor; this->mAPIVersion.patch = patch; }

		virtual void setAPIVersion(APIVersion version);

		virtual APIVersion getAPIVersion() const;

		bool areDebuggingToolsEnabled() const;

		bool isErrorCheckingEnabled() const;

		virtual void init() = 0;

		virtual void update() = 0;
		
		virtual void shutdown() = 0;

		template<typename T_CAST_TO>
		T_CAST_TO* castAs() {
			return dynamic_cast<T_CAST_TO*>(this);
		}
	};

#ifdef ATLAS_USE_VULKAN
	
	class VulkanRenderingBackend : public RenderingBackend {
	private:
		class ValidationLayers {
		private:

			std::vector<const char*> mValidationLayers;

		public:

			ValidationLayers(std::initializer_list<const char*> layers) : mValidationLayers(layers) {}

			/**
			 * @brief Gets a copy of the validation layers vector.
			 *
			 * @return
			 *
			 * @since v
			 */
			std::vector<const char*> getLayers() const {
				return mValidationLayers;
			}

		};

		ValidationLayers* mValidationLayers = nullptr;
		
		// Vulkan stuff
		VkInstance mVulkanInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT mVulkanDebugMessenger = VK_NULL_HANDLE;
		VkPhysicalDevice mVulkanPhysicalDevice = VK_NULL_HANDLE;
		VkDevice mVulkanDevice = VK_NULL_HANDLE;
		VkSurfaceKHR mVulkanSurface = VK_NULL_HANDLE;

		//std::vector<const char*> mValidationLayers;

		uint16_t initInstance(const APIVersion& cAPIVersionRef, bool cbEnableValidationLayers, std::string appName);

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