/**************************************************************************************************
 * @file VKDevice.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

// This avoids the transitive include of string_view on MSVC compilers
#ifdef ATLAS_COMPILER_MSVC
	#include <__msvc_string_view.hpp>
#endif

#include <filesystem>
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <iosfwd>
#include <span>
#include <__msvc_string_view.hpp>
#include <vk_mem_alloc.h>
#include <iostream>
#include <fstream>
#include <bitset>

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	#include <vk_mem_alloc.h>
#endif

#include "RenderingBackend.h"

#include "../VulkanInstance.h"
#include "../Frame.h"
#include "../AllocatedImage.h"
#include "../../core/Core.h"
#include "../../core/Device.h"
#include "../window/Window.h"
#include "../DeletionQueue.h"
#include "../DescriptorLayoutBuilder.h"

#define ATLAS_1_SECOND_IN_NS 1000000000

#define ATLAS_VK_DEVICE_BITS 32
//std::cout << "Detected Vulkan error: " << std::string(string_VkResult(err)) << std::endl; 
#define VK_CHECK(x)                                                     


/*    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
			
            abort();                                                    \
        }                                                               \
    } while (0)*/

#ifdef ATLAS_USE_VULKAN

namespace Atlas {


	//struct DescriptorLayoutBuilder {

	//	std::vector<VkDescriptorSetLayoutBinding> bindings;

	//	void add_binding(uint32_t binding, VkDescriptorType type)
	//	{
	//		VkDescriptorSetLayoutBinding newbind{};
	//		newbind.binding = binding;
	//		newbind.descriptorCount = 1;
	//		newbind.descriptorType = type;

	//		bindings.push_back(newbind);
	//	}

	//	void clear()
	//	{
	//		bindings.clear();
	//	}

	//	VkDescriptorSetLayout build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0)
	//	{
	//		for (auto& b : bindings) {
	//			b.stageFlags |= shaderStages;
	//		}

	//		VkDescriptorSetLayoutCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	//		info.pNext = pNext;

	//		info.pBindings = bindings.data();
	//		info.bindingCount = (uint32_t)bindings.size();
	//		info.flags = flags;

	//		VkDescriptorSetLayout set;
	//		vkCreateDescriptorSetLayout(device, &info, nullptr, &set);

	//		return set;
	//	}
	//};

	struct ImmediateSubmitInfo {
		VkFence fence;
		VkCommandBuffer commandBuffer;
		VkCommandPool commandPool;
	};

	class DescriptorAllocator {
	public:
		struct PoolSizeRatio {
			VkDescriptorType type;
			float ratio;
		};

		VkDescriptorPool pool;

		void init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
		void clear_descriptors(VkDevice device);
		void destroy_pool(VkDevice device);

		VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);
	};

	class Shader {
	private:
		std::filesystem::path mCompiledShaderPath;
		std::string mName;

		VkShaderModule mShaderModule = VK_NULL_HANDLE;
	public:

		Shader() = default;

		explicit Shader(std::filesystem::path compiledShaderPath, std::string name);

		void destroyModule(VkDevice* device);

		bool load(VkDevice* device);

		VkShaderModule getModule() const;

	};

	class Pipeline {
	private:
		VkPipeline mPipeline = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		Shader mShader;
	public:

		void createLayout(VkPipelineLayoutCreateInfo layoutInfo, VkDevice* device);

		void createLayout(VkDevice* device, VkDescriptorSetLayout* descriptorSetLayout);

		void init(VkDevice* device, DeletionQueue* deletionQueue, VkPipelineShaderStageCreateInfo stageInfo, VkComputePipelineCreateInfo pipelineInfo) {
			vkCreateComputePipelines(*device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline);

			vkDestroyShaderModule(*device, mShader.getModule(), nullptr);

			deletionQueue->push([&]() {
				destroyPipeline(device);
			});
		}

		void destroyPipeline(VkDevice* device) {
			vkDestroyPipelineLayout(*device, mPipelineLayout, nullptr);
			vkDestroyPipeline(*device, mPipeline, nullptr);
		}
	};

	class Renderable {
	public:

	};

	class BackgroundColor : public Renderable {
	public:

	};

	enum class EVulkanRenderingOption : size_t {
		Version_Major = 0,
		Version_Minor,
		Version_Patch,
		Enable_Validation_Layers,
		Use_Dynamic_Rendering,
		Use_Default_Instance_Builder,
		Enable_Syncronization,
		Enable_Buffer_Device_Address,
		Enable_Descriptor_Indexing,
		Swapchain_Format,
		Fence_Timeout,
	};

	/**
	 * @brief A wrapper class for the usage of Vulkan as a rendering backend. This class handles the lifecycle of Vulkan and simplifies the usage of Vulkan to a simple API.
	 * 
	 * @pre The window must be set up and open prior to initializing Vulkan (and therefore this class!).
	 * 
	 * @warning This class requires the user's device to support Vulkan!
	 * 
	 * @since v0.0.1
	 */
	class VulkanRenderingBackend : public RenderingBackend {
	private:

		std::bitset<ATLAS_VK_DEVICE_BITS> mOptionsBitset;

		int mCurrentFrameNumber = 0;

		bool mIsInitialized = false;
		bool mbUseDefaultInstanceBuilder = true;

		ImmediateSubmitInfo mImmediateSubmitInfo;

		VulkanInstance mInstance;

		// Vulkan stuff
		//VkInstance mVulkanInstance = VK_NULL_HANDLE;
		//VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
		VkPhysicalDevice mGPUDevice = VK_NULL_HANDLE;
		VkDevice mDevice = VK_NULL_HANDLE;
		VkSurfaceKHR mSurface = VK_NULL_HANDLE;

		VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
		VkFormat mSwapchainImageFormat;

		std::vector<VkImage> mSwapchainImages;
		std::vector<VkImageView> mSwapchainImageViews;
		VkExtent2D mSwapchainExtent;

		// TODO: Move to a frame manager class or something like that
		std::array<FrameData, FRAME_OVERLAP> mFrameDataArray;

		AllocatedImage mDrawImage;
		VkExtent2D mDrawExtent;

		VkQueue mGraphicsQueue = VK_NULL_HANDLE;
		uint32_t mGraphicsQueueFamily;

		DeletionQueue mMainDeletionQueue;
		VmaAllocator mVMAAllocator = VK_NULL_HANDLE;

		uint64_t mFencesTimeoutNS = ATLAS_1_SECOND_IN_NS;
		uint64_t mNextImageTimeoutNS = ATLAS_1_SECOND_IN_NS;

		DescriptorAllocator mGlobalDescriptorAllocator;

		VkDescriptorSet mDrawImageDescriptors;
		VkDescriptorSetLayout mDrawImageDescriptorLayout;

		VkPipeline mGradientPipeline;
		VkPipelineLayout mGradientPipelineLayout;

		std::string mApplicationName;

		uint16_t initInstance(const APIVersion& cAPIVersionRef, const bool cbEnableValidationLayers, std::string const& appName);
		
		void initDescriptors();

		void initPipelines();
		void initBackgroundPipelines();
	
		void initVMAAllocator(vkb::Instance const& cVkBootstrapInstanceRef);

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

		void setOption(EVulkanRenderingOption option, int value) {
			mOptionsBitset[static_cast<size_t>(option)] = value;
		}

		/**
		 * @brief Initializes Vulkan.
		 * 
		 * @param windowHandle The currently @b initialized and @b open window pointer.
		 * 
		 * @pre You must make sure that the window is @b initialized and @b open before calling this!
		 * 
		 * @note This can take some time to complete. Further testing should be done to see if this is a problem.
		 * 
		 * @since v0.0.1
		 */
		void init(AGameWindow* gameWindow) override;
		
		void initSwapchain(AGameWindow* gameWindow);

		void initCommands();

		void update() override;

		void resetFences(const uint32_t cFenceCount, FrameData& currentFrame);

		void draw();

		void drawBackground(VkCommandBuffer cmd);

		void present();

		void shutdown() override;
		
		void shouldUseDefaultInstanceBuilder(bool bUseDefaultInstanceBuilder);

		void setApplicationName(std::string_view applicationName);

		void setFenceTimeout(uint64_t lengthInNS);

		void setNextImageTimeout(uint64_t lengthInNS);

		void createSwapchain(uint32_t width, uint32_t height);
		
		void destroySwapchain();

		void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);

		bool checkValidationLayerSupport();

		bool canInitialize(AGameWindow* gameWindow);

		std::string getApplicationName();

		FrameData& getCurrentFrame();
	};

	VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
	VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1);

	VkFenceCreateInfo CreateFenceCreateInfo(VkFenceCreateFlags flags = 0 /*= 0*/);

	VkSemaphoreCreateInfo CreateSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0 /*= 0*/);
	
	VkSemaphoreSubmitInfo SubmitSemaphoreInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

	VkCommandBufferSubmitInfo SubmitCommandBufferInfo(VkCommandBuffer cmd);

	VkSubmitInfo2 SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
		VkSemaphoreSubmitInfo* waitSemaphoreInfo);

	VkCommandBufferBeginInfo CreateCommandBufferBeginInfo(VkCommandBufferUsageFlags flags /*= 0*/);

	void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
	VkImageSubresourceRange CreateImageSubresourceRange(VkImageAspectFlags aspectMask);

	VkImageCreateInfo CreateImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);

	VkImageViewCreateInfo CreateImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

	void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);

	bool LoadShaderModule(const char* filePath,
	VkDevice device,
	VkShaderModule* outShaderModule);

	VulkanRenderingBackend& getLoadedRenderingBacked();

	void setLoadedRenderingBackend(VulkanRenderingBackend* backend);
	void resetLoadedRenderingBackend();

}
#endif