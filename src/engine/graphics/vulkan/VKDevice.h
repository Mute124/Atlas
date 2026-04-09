/**************************************************************************************************
 * @file VKDevice.h
 * 
 * @brief This file provides declarations that are specific to the Vulkan rendering backend implementation. 
 * 
 * @date September 2025
 * 
 * @since v
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
	
#include "../../core/Math.h"

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
#include "AllocatedBuffer.h"
#include "GraphicsQueue.h"


#include "../Frame.h"

#include "../backend/RenderingBackend.h"

#include "../drawing/EffectManager.h"

#include "../window/Window.h"

#include "../../core/Core.h"
#include "../../core/Common.h"
#include "../../core/Device.h"
#include "../../core/ApplicationInfo.h"

#include "../../debugging/Logging.h"

#include "../../io/IOManager.h"

#include "../drawing/DrawData.h"
#include "../drawing/PipelineLayout.h"
#include "../drawing/Pipeline.h"
#include "../drawing/RenderPass.h"
#include "../drawing/Shader.h"
#include "../drawing/RenderPassesManager.h"
#include "../drawing/Renderable.h"
#include "../drawing/Viewport.h"

#include "../../time/FPS.h"

#include "../Mesh.h"
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

	struct ImmediateSubmitInfo {
		VkFence fence;
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		VkCommandPool commandPool;
	};


	struct DescriptorAllocatorGrowable {
	public:
		struct PoolSizeRatio {
			VkDescriptorType type;
			float ratio;
		};
	private:
		VkDescriptorPool get_pool(VkDevice device);
		VkDescriptorPool create_pool(VkDevice device, uint32_t setCount, std::span<PoolSizeRatio> poolRatios);

		std::vector<PoolSizeRatio> ratios;
		std::vector<VkDescriptorPool> fullPools;
		std::vector<VkDescriptorPool> readyPools;
		uint32_t setsPerPool;

	public:


		void init(VkDevice device, uint32_t initialSets, std::span<PoolSizeRatio> poolRatios);
		void clear_pools(VkDevice device);
		void destroy_pools(VkDevice device);

		VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout, void* pNext = nullptr);

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

	class BackgroundRenderPass : public RenderPass {
	public:
		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override {
			//ComputeEffect& effect = mPipeline->mShader.mComputeEffects.getCurrentEffect();

			ComputeEffect& effect = cDrawData.computeEffects->getCurrentEffect();

			effect.bind(cDrawData.cmd);

			//vkCmdBindDescriptorSets(cDrawData.cmd, VK_PIPELINE_BIND_POINT_COMPUTE);
		}
	};

	class ImGuiRenderable : public Renderable {
	public:


		ImGuiRenderable(std::string const& name) : Renderable(name) {}
		ImGuiRenderable() : ImGuiRenderable("Unnamed ImGuiRenderable") {}

		virtual bool setupElements(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);
	};

	class ImGuiWindowRenderable : public ImGuiRenderable {
	public:

		ImGuiWindowRenderable(std::string const& name) : ImGuiRenderable(name) {}
		ImGuiWindowRenderable() : ImGuiWindowRenderable("Unnamed ImGuiWindowRenderable") {}

		virtual bool setupElements(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;

		virtual bool beginDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData, EffectManager& computeEffects);

		virtual bool endDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData);
	};

	class IMGUIRenderPass : public RenderPass {
	private:
		std::vector<std::shared_ptr<ImGuiRenderable>> mIMGUIRenderables;
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
			addImGuiRenderable(std::make_shared<ImGuiRenderable>(ImGuiRenderable()));
		}

		void addImGuiRenderable(std::shared_ptr<ImGuiRenderable> renderable) {
			mIMGUIRenderables.push_back(renderable);
		}

		virtual void beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;

		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;

		virtual void endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) override;
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
	class VulkanRenderingBackend : public ARenderingBackend {
	public:
		static inline double Now() { return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count(); }

		// Thread-safe global instance of this class
		static inline std::shared_ptr<VulkanRenderingBackend> sInstance = nullptr;
		static inline std::mutex sInstanceMutex;
		
		std::vector<ComputeEffect> mBackgroundEffects;
		int mCurrentBackgroundEffect = 0;

		std::shared_ptr<EffectManager> mEffectManager;
		RenderPassesManager mRenderPassesManager;

		CurrentDrawData mCurrentDrawData;

		std::vector<std::shared_ptr<MeshAsset>> testMeshes;

		int mCurrentFrameNumber = 0;

		FPS mFrameTime{};

		bool mIsInitialized = false;
		bool mbUseDefaultInstanceBuilder = true;
		bool mbResizeRequested = false;

		ImmediateSubmitInfo mImmediateSubmitInfo;

		VulkanInstanceWrapper mInstance;

		PhysicalDevice mPhysicalDevice;

		VkSurfaceKHR mSurface = VK_NULL_HANDLE;

		vkb::Device mDevice;

		VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
		VkFormat mSwapchainImageFormat;

		std::vector<VkImage> mSwapchainImages;
		std::vector<VkImageView> mSwapchainImageViews;
		VkExtent2D mSwapchainExtent;

		// TODO: Move to a frame manager class or something like that
		std::array<FrameData, FRAME_OVERLAP> mFrameDataArray;
		glm::mat4 mViewMatrix = glm::mat4(1.0f);

		AllocatedImage mDepthImage;
		AllocatedImage mDrawImage;
		
		VkExtent2D mDrawExtent;
		static inline float mRenderScale = 1.0f;

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

		std::string mApplicationName;

		ApplicationInfo mAppInfo{};

		ComputeEffect sky;
		ComputeEffect gradient;

		void initDescriptors();

		void initPipelines();
		void initBackgroundPipelines();
		void initTrianglePipeline();
		void initMeshPipeline();

		void initVMAAllocator(vkb::Instance const& cVkBootstrapInstanceRef);

		friend class GameWindow;

	public:

		using ARenderingBackend::ARenderingBackend;
		
		static inline float sFOVY = 120.0f;
		static inline float sClipNear = 10000.0f;
		static inline float sClipFar = 10.1f;


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

		VulkanRenderingBackend(const ApplicationInfo& appInfo, GameWindow* gameWindow);

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
		void init(GameWindow* gameWindow) override;

		void initDefaultData();

		void initPhysicalDevice();

		void initSwapchain(GameWindow* gameWindow);

		void initSyncStructures();

		void initCommands();

		void initIMGUI(GameWindow* gameWindow);

		void resetFences(const uint32_t cFenceCount, FrameData& currentFrame);

		void beginDrawingMode();

		void draw();

		void endDrawingMode();

		void drawIMGUI(VkCommandBuffer cmd, VkImageView targetImageView);

		void drawBackground(VkCommandBuffer cmd);

		void drawGeometry(VkCommandBuffer cmd);

		void shutdown() override;
		
		void shouldUseDefaultInstanceBuilder(bool bUseDefaultInstanceBuilder);

		void setApplicationName(std::string_view applicationName);

		void setFenceTimeout(uint64_t lengthInNS);

		void setNextImageTimeout(uint64_t lengthInNS);

		void createSwapchain(uint32_t width, uint32_t height);

		void destroySwapchain();

		void resize(GameWindow* gameWindow)
		{
			vkDeviceWaitIdle(mDevice);

			destroySwapchain();
			
			int w, h;
			SDL_GetWindowSize(gameWindow->getWindowHandle(), &w, &h);
			mDrawExtent.width = w;
			mDrawExtent.height = h;

			createSwapchain(mDrawExtent.width, mDrawExtent.height);

			mbResizeRequested = false;
		}
		GPUMeshBuffers UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

		void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);

		bool canInitialize(GameWindow* gameWindow);

		std::string getApplicationName();

		FrameData& getCurrentFrame();

		VkSurfaceKHR getSurface() { return mSurface; }

		PhysicalDevice& getPhysicalDevice() { return mPhysicalDevice; }

		bool isResizeRequested() { return mbResizeRequested; }
	};



	VulkanRenderingBackend& getLoadedRenderingBacked();

	void setLoadedRenderingBackend(VulkanRenderingBackend* backend);
	void resetLoadedRenderingBackend();

	std::optional<std::vector<std::shared_ptr<MeshAsset>>> loadGltfMeshes(VulkanRenderingBackend* engine, std::filesystem::path filePath);
	

}
#endif