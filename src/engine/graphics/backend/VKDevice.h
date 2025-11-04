/**************************************************************************************************
 * @file VKDevice.h
 * 
 * @brief This file provides declarations that are specific to the Vulkan rendering backend implementation. 
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
#include <memory>

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	#include <vk_mem_alloc.h>
#endif

#include "RenderingBackend.h"

#include "../VulkanInstance.h"
#include "../PhysicalDevice.h"

#include "../Frame.h"
#include "../AllocatedImage.h"
#include "../../core/Core.h"
#include "../../core/Common.h"
#include "../../core/Device.h"
#include "../window/Window.h"
#include "../DeletionQueue.h"
#include "../DescriptorLayoutBuilder.h"
#include "../../debugging/Logging.h"
#include "../GraphicsUtils.h"
#include "../PipelineBuilder.h"

#include <VkBootstrap.h>
#include <functional>
#include <mutex>
#include <glm/fwd.hpp>
#include <vulkan/vulkan_core.h>

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

	enum class QueueType {
		Present = 0,
		Graphics = 1,
		Compute = 2,
		Transfer = 3
	};

	struct ImmediateSubmitInfo {
		VkFence fence;
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		VkCommandPool commandPool;
	};

	struct CurrentDrawData {
		const static uint32_t FENCE_COUNT = 1;
		uint32_t swapchainImageIndex;
		VkCommandBufferResetFlags cmdResetFlags = 0;
		VkCommandBuffer cmd = VK_NULL_HANDLE;
		VkCommandBufferBeginInfo cmdBeginInfo = {};
	};

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

	struct AllocatedBuffer {
		VkBuffer buffer;
		VmaAllocation allocation;
		VmaAllocationInfo info;
	};

	struct Vertex {

		glm::vec3 position;
		float uv_x;
		glm::vec3 normal;
		float uv_y;
		glm::vec4 color;
	};

	// holds the resources needed for a mesh
	struct GPUMeshBuffers {

		AllocatedBuffer indexBuffer;
		AllocatedBuffer vertexBuffer;
		VkDeviceAddress vertexBufferAddress;
	};

	// push constants for our mesh object draws
	struct GPUDrawPushConstants {
		glm::mat4 worldMatrix;
		VkDeviceAddress vertexBuffer;
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
	protected:
		VkPipelineLayoutCreateInfo mComputeLayoutInfo;
		VkPushConstantRange mPushConstantRange;

		VkPipeline mPipeline = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		Shader mShader;
	public:
		Pipeline() = default;


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
	protected:
		friend class VulkanRenderingBackend;


	public:

		static inline void SetCurrentDrawData(CurrentDrawData const& currentFrame) { 
			//std::scoped_lock lock(sCurrentDrawDataMutex);
			//sCurrentDrawData = currentFrame;
		}

		virtual void draw(VkCommandBuffer cmd) {
			
		}
	};

	class IMGUIRenderable : public Renderable {
	public:

		void draw(VkCommandBuffer cmd) override
		{
		}
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

	class GraphicsQueue {
	private:
		VkQueue mQueue{ VK_NULL_HANDLE };
		uint32_t mQueueFamily{ 1 };

	public:
		GraphicsQueue(VkQueue queue, uint32_t queueFamily);
		
		GraphicsQueue() = default;

		void submit(std::span<VkSubmitInfo2> submitInfos, VkFence fence = VK_NULL_HANDLE);

		void submit(VkSubmitInfo2 const& submitInfo, VkFence fence = VK_NULL_HANDLE);

		VkQueue& getQueue() {
			return mQueue;
		}

		ATLAS_IMPLICIT operator VkQueue& () {
			const_cast<Atlas::GraphicsQueue&>(*this).getQueue();
		}
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
		// Thread-safe global instance of this class
		static inline std::shared_ptr<VulkanRenderingBackend> sInstance = nullptr;
		static inline std::mutex sInstanceMutex;
		
		std::vector<ComputeEffect> mBackgroundEffects;
		int mCurrentBackgroundEffect = 0;

		CurrentDrawData mCurrentDrawData;

		//std::bitset<ATLAS_VK_DEVICE_BITS> mOptionsBitset;

		int mCurrentFrameNumber = 0;

		bool mIsInitialized = false;
		bool mbUseDefaultInstanceBuilder = true;

		ImmediateSubmitInfo mImmediateSubmitInfo;

		VulkanInstanceWrapper mInstance{};

		// Vulkan stuff
		//VkInstance mVulkanInstance = VK_NULL_HANDLE;
		//VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
		PhysicalDevice mPhysicalDevice;
		
		//VkPhysicalDevice mGPUDevice = VK_NULL_HANDLE;
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

		//GraphicsQueue mGraphicsQueue;

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

		VkPipelineLayout _trianglePipelineLayout;
		VkPipeline _trianglePipeline;

		VkPipelineLayout _meshPipelineLayout;
		VkPipeline _meshPipeline;

		GPUMeshBuffers rectangle;

		std::string mApplicationName;


		uint16_t initInstance(const APIVersion& cAPIVersionRef, const bool cbEnableValidationLayers, std::string const& appName);
		
		void initDescriptors();

		void initPipelines();
		void initBackgroundPipelines();
		void initTrianglePipeline();
		void initMeshPipeline();

		void initVMAAllocator(vkb::Instance const& cVkBootstrapInstanceRef);

		friend class SDLGameWindow;

	protected:
		//static inline void SetGlobalInstance(std::shared_ptr<VulkanRenderingBackend> instance) {
		//	std::scoped_lock lock(sInstanceMutex);
		//	sInstance = instance; 
		//}

	public:

		using RenderingBackend::RenderingBackend;

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

		//void setOption(EVulkanRenderingOption option, int value) {
		//	mOptionsBitset[static_cast<size_t>(option)] = value;
		//}
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
		
		void initDefaultData();

		void initPhysicalDevice();

		void initSwapchain(AGameWindow* gameWindow);

		void initCommands();

		void initIMGUI(AGameWindow* gameWindow);

		void resetFences(const uint32_t cFenceCount, FrameData& currentFrame);

		void beginDrawingMode();

		void endDrawingMode();

		void draw();

		void drawIMGUI(VkCommandBuffer cmd, VkImageView targetImageView);

		void drawBackground(VkCommandBuffer cmd);

		void drawGeometry(VkCommandBuffer cmd);

		void present();

		void shutdown() override;
		
		void shouldUseDefaultInstanceBuilder(bool bUseDefaultInstanceBuilder);

		void setApplicationName(std::string_view applicationName);

		void setFenceTimeout(uint64_t lengthInNS);

		void setNextImageTimeout(uint64_t lengthInNS);

		AllocatedBuffer createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		void destroyBuffer(const AllocatedBuffer& buffer);

		void createSwapchain(uint32_t width, uint32_t height);
		
		void destroySwapchain();

		GPUMeshBuffers UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

		void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);

		bool checkValidationLayerSupport();

		bool canInitialize(AGameWindow* gameWindow);

		std::string getApplicationName();

		FrameData& getCurrentFrame();


	};



	VulkanRenderingBackend& getLoadedRenderingBacked();

	void setLoadedRenderingBackend(VulkanRenderingBackend* backend);
	void resetLoadedRenderingBackend();



}
#endif