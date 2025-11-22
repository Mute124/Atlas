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

#define GLM_ENABLE_EXPERIMENTAL


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
#include <functional>
#include <mutex>
#include <unordered_map>

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>

	#include <vk_mem_alloc.h>

	#include <VkBootstrap.h>
#endif
	
#include <glm/gtx/quaternion.hpp>
#include <glm/fwd.hpp>

#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/parser.hpp>
#include <fastgltf/tools.hpp>

#include "stb_image.h"

#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "DeletionQueue.h"
#include "DescriptorLayoutBuilder.h"
#include "../GraphicsUtils.h"
#include "PipelineBuilder.h"
#include "AllocatedImage.h"

#include "../Frame.h"

#include "../backend/RenderingBackend.h"

#include "../drawing/EffectManager.h"

#include "../window/Window.h"

#include "../../core/Core.h"
#include "../../core/Common.h"
#include "../../core/Device.h"

#include "../../debugging/Logging.h"

#include "../../io/IOManager.h"

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
	enum class QueueType {
		Present = 0,
		Graphics = 1,
		Compute = 2,
		Transfer = 3
	};

	enum class EShaderModuleType {
		Vertex = 0,
		Fragment = 1,
		Compute = 2,
		Geometry = 3
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

		std::shared_ptr<EffectManager> computeEffects;

		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		VkFormat swapchainImageFormat;

		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		//VkExtent2D swapchainExtent;

		// TODO: Move to a frame manager class or something like that
		std::array<FrameData, FRAME_OVERLAP> frameDataArray;

		VkImageView currentSwapchainImageView;
		VkExtent2D swapchainExtent;
		
		AllocatedImage drawImage;
		VkExtent2D drawExtent;
		VkDescriptorSet* drawImageDescriptors;
	};

	//struct ComputePushConstants {
	//	glm::vec4 data1;
	//	glm::vec4 data2;
	//	glm::vec4 data3;
	//	glm::vec4 data4;
	//};

	//struct ComputeEffect {
	//	const char* name;

	//	VkPipeline pipeline;
	//	VkPipelineLayout layout;

	//	ComputePushConstants data;
	//};

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

	struct GeoSurface {
		uint32_t startIndex;
		uint32_t count;
	};

	struct MeshAsset {
		std::string name;

		std::vector<GeoSurface> surfaces;
		GPUMeshBuffers meshBuffers;
	};



	class CommandBuffer : public AVulkanHandleWrapper<VkCommandBuffer> {
	public:
		using BeginInfo = VkCommandBufferBeginInfo;
		using EResetFlag = VkCommandBufferResetFlags;
		
		CommandBuffer(VkCommandBuffer handle) : AVulkanHandleWrapper<VkCommandBuffer>(handle) {}

		void begin(BeginInfo const& createInfo) {
			vkBeginCommandBuffer(getHandle(), &createInfo);
		}

		void end() { 
			vkEndCommandBuffer(getHandle());
		}

		void reset(EResetFlag flags = 0) {
			vkResetCommandBuffer(getHandle(), flags);
		}

		//void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
		//	vkCmdDispatch(getHandle(), groupCountX, groupCountY, groupCountZ);
		//}
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

	class ShaderModule {
	private:
		friend class ShaderBase;

		Device mDevice;

		VkShaderModule mShaderModule{ VK_NULL_HANDLE };
		EShaderModuleType mModuleType{ EShaderModuleType::Vertex };

		bool mbLoaded{ false };
	public:

		ShaderModule(Device const& device, std::filesystem::path const& path, FileManager& ioManager, EShaderModuleType moduleType);

		ShaderModule() = default;
		
		bool createShaderModule(Device const& device, VkShaderModuleCreateInfo* createInfo, VkShaderModule* module);

		bool load(std::filesystem::path const& path, FileManager& ioManager);

		bool destroy();

		bool isLoaded() const;

		VkShaderModule getShaderModule() const;

		EShaderModuleType getShaderModuleType() const;

		explicit(false) operator VkShaderModule() const;
	};

	class ShaderBase : public NamedObject {
	private:
		std::vector<ShaderModule> mShaderModules;
		
	public:

		void pushShaderModule(ShaderModule shaderModule) { 
			mShaderModules.push_back(shaderModule);
		}
		
		void destroy() {
			for (auto& shaderModule : mShaderModules) {
				shaderModule.destroy();
			}
		}
	};

	// A grouping of shader modules
	class Shader : public ShaderBase {
	private:

		VkShaderModule mShaderModule = VK_NULL_HANDLE;
		//std::vector<VkShaderModule> mShaderModules;
		

	public:

		Shader() = default;

		//explicit Shader(std::filesystem::path compiledShaderPath, std::string name) : mCompiledShaderPath(compiledShaderPath), mName(name) {}

		//void destroyModule(VkDevice device) {
		//	vkDestroyShaderModule(device, mShaderModule, nullptr);
		//}

		//bool load(VkDevice device) {
		//	return LoadShaderModule(mCompiledShaderPath.string().c_str(), device, &mShaderModule);
		//}

		VkShaderModule getModule() const;

	};

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
		
		void createPipelineLayout(VkDevice device, LayoutCreateInfo const& layoutInfo);

	public:
		PipelineLayout(VkPipelineLayout handle) : AVulkanHandleWrapper<VkPipelineLayout>(handle) {}

		PipelineLayout(Device device, LayoutCreateInfo layoutInfo);

		PipelineLayout() = default;


	};

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

	using RenderPassIndex = size_t;

	class RenderPass : public NamedObject, public Validatable {
		friend class RenderPassesManager;
	private:
		std::shared_ptr<Pipeline> mPipeline;

		RenderPassIndex mIndex;

	protected:
		void setIndex(RenderPassIndex index) { mIndex = index; }
	public:
		//RenderPass(VkCommandBuffer commandBuffer) : mCommandBuffer(commandBuffer) {}

		RenderPass() = default;

		virtual void beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			
		}

		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) = 0;

		virtual void endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {

		}

		RenderPassIndex getID() const { return mIndex; }
	};

	class RenderPassesManager {
	private:
		std::vector<std::shared_ptr<RenderPass>> mRenderPasses;
		std::shared_mutex mRenderPassesMutex;

	public:

		void addRenderPass(std::shared_ptr<RenderPass> renderPass) {
			if (renderPass == nullptr || !renderPass->isValid())
			{
				ErrorLog(std::format("RenderPass is not valid: {}", renderPass->getName()));
				
				return;
			}

			std::unique_lock lock(mRenderPassesMutex);
			
			const RenderPassIndex index = mRenderPasses.size();
			renderPass->setIndex(index);

			mRenderPasses.push_back(renderPass);

		}

		void beginDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);
			
			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{								
					continue;
				}

				renderPass->beginRenderPass(cmd, cDrawData);
			}
		}

		void drawRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);

			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{
					continue;
				}

				renderPass->draw(cmd, cDrawData);

			}
		}

		void endDrawingRenderPasses(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			std::unique_lock lock(mRenderPassesMutex);

			for (auto& renderPass : mRenderPasses) {
				if (!renderPass->isValid())
				{
					continue;
				}

				renderPass->endRenderPass(cmd, cDrawData);

			}
		}
	};

	class BackgroundRenderPass : public RenderPass {
	public:
		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override {
			//ComputeEffect& effect = mPipeline->mShader.mComputeEffects.getCurrentEffect();

			ComputeEffect& effect = cDrawData.computeEffects->getCurrentEffect();

			effect.bind(cDrawData.cmd);

			//vkCmdBindDescriptorSets(cDrawData.cmd, VK_PIPELINE_BIND_POINT_COMPUTE);
		}
	};

	class IMGUIRenderPass : public RenderPass {
	protected:
		virtual void setupFrameElements(EffectManager& computeEffects) {
			//ComputeEffect& selected = computeEffects.getCurrentEffect();

			//ImGui::Text("Selected effect: ", selected.name);

			////ImGui::SliderInt("Effect Index", &computeEffects.getCurrentEffectIndex(), 0, mBackgroundEffects.size() - 1);

			//ImGui::InputFloat4("data1", (float*)&selected.data.data1);
			//ImGui::InputFloat4("data2", (float*)&selected.data.data2);
			//ImGui::InputFloat4("data3", (float*)&selected.data.data3);
			//ImGui::InputFloat4("data4", (float*)&selected.data.data4);
		}
	public:

		IMGUIRenderPass() : RenderPass() {
			this->setValid();
		}

		void beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;

		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;

		virtual void endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;
	};

	class Renderable {
	protected:
		friend class VulkanRenderingBackend;


	public:

		static inline void SetCurrentDrawData(CurrentDrawData const& currentFrame) { 
			//std::scoped_lock lock(sCurrentDrawDataMutex);
			//sCurrentDrawData = currentFrame;
		}
		
		virtual void beginDrawingStage(VkCommandBuffer cmd, EffectManager& computeEffects) {
		}

		virtual void draw(VkCommandBuffer cmd) {
			
		}

		virtual void endDrawingStage(VkCommandBuffer cmd) {
		}
	};

	class IMGUIRenderable : public Renderable {
	private:
		static inline void NewIMGUIFrame();

	protected:

		virtual void setupElements(EffectManager& computeEffects);
	public:

		void beginDrawingStage(VkCommandBuffer cmd, EffectManager& computeEffects) override;

		void draw(VkCommandBuffer cmd) override;

		void endDrawingStage(VkCommandBuffer cmd) override;
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
	public:

		// Thread-safe global instance of this class
		static inline std::shared_ptr<VulkanRenderingBackend> sInstance = nullptr;
		static inline std::mutex sInstanceMutex;
		
		std::vector<ComputeEffect> mBackgroundEffects;
		int mCurrentBackgroundEffect = 0;

		std::shared_ptr<EffectManager> mEffectManager;
		RenderPassesManager mRenderPassesManager;

		CurrentDrawData mCurrentDrawData;

		std::vector<std::shared_ptr<MeshAsset>> testMeshes;

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
		//VkDevice mDevice = VK_NULL_HANDLE;
		VkSurfaceKHR mSurface = VK_NULL_HANDLE;

		Device mDevice;

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

		VkSurfaceKHR getSurface() { return mSurface; }

		PhysicalDevice& getPhysicalDevice() { return mPhysicalDevice; }
	};



	VulkanRenderingBackend& getLoadedRenderingBacked();

	void setLoadedRenderingBackend(VulkanRenderingBackend* backend);
	void resetLoadedRenderingBackend();

	std::optional<std::vector<std::shared_ptr<MeshAsset>>> loadGltfMeshes(VulkanRenderingBackend* engine, std::filesystem::path filePath);

}
#endif