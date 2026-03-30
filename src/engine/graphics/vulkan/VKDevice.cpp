/**************************************************************************************************
 * @file VKDevice.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "VKDevice.h"
#include "VulkanInstance.h"
#include "../backend/RenderingBackend.h"
#include "../Frame.h"
#include "../../core/Core.h"
#include "../window/Window.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#define STB_IMAGE_IMPLEMENTATION

//#include "vk_mem_alloc.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_vulkan.h>


#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <span>

// This avoids the transitive include of string_view on MSVC compilers
#ifdef ATLAS_COMPILER_MSVC
	#include <__msvc_string_view.hpp>
#endif // ATLAS_COMPILER_MSVC

#ifdef ATLAS_USE_SDL2
	#include <SDL2/SDL_video.h>
#endif // ATLAS_USE_SDL2

#ifdef ATLAS_USE_VULKAN
		#include <vulkan/vulkan_core.h>


	#include <VkBootstrap.h>

	#ifdef ATLAS_USE_SDL2
		#include <SDL_vulkan.h>
	#endif // ATLAS_USE_SDL2
#include "../../core/Common.h"
#include "../../debugging/Logging.h"
#include "DescriptorLayoutBuilder.h"

#include <cmath>
#include <filesystem>
#include <format>
#include <functional>


#endif // ATLAS_USE_VULKAN
#include "../../core/Version.h"
#include "../GraphicsUtils.h"
#include "PipelineBuilder.h"
#include <glm/fwd.hpp>
#include "PhysicalDevice.h"
#include <array>
#include <cstring>
#include <fastgltf/util.hpp>
#include "../drawing/Effect.h"
#include "../drawing/EffectManager.h"
#include <optional>
#include <type_traits>
#include <glm/gtx/transform.hpp>



#ifdef ATLAS_USE_VULKAN

using namespace Atlas;

struct GlobalContext {
	Version cApiVersion;
};

std::unique_ptr<GlobalContext> gGlobalContext;

Atlas::VulkanRenderingBackend* gLoadedVulkanBackend = nullptr;

static inline glm::vec3 sCameraPosition = { 0.f, 0.f, -1.f };
static inline int sModelId = 0;

void Atlas::DescriptorAllocator::init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios)
{	

	std::vector<VkDescriptorPoolSize> poolSizes;

	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * maxSets)
		});
	}

	VkDescriptorPoolCreateInfo pool_info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	pool_info.flags = 0;
	pool_info.maxSets = maxSets;
	pool_info.poolSizeCount = (uint32_t)poolSizes.size();
	pool_info.pPoolSizes = poolSizes.data();

	vkCreateDescriptorPool(device, &pool_info, nullptr, &pool);
}

void Atlas::DescriptorAllocator::clear_descriptors(VkDevice device)
{
	vkResetDescriptorPool(device, pool, 0);
}

void Atlas::DescriptorAllocator::destroy_pool(VkDevice device)
{
	vkDestroyDescriptorPool(device, pool, nullptr);
}

VkDescriptorSet Atlas::DescriptorAllocator::allocate(VkDevice device, VkDescriptorSetLayout layout)
{
	VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet ds;
	vkAllocateDescriptorSets(device, &allocInfo, &ds);

	return ds;
}

void Atlas::VulkanRenderingBackend::setApplicationName(std::string_view applicationName) {
	mApplicationName = applicationName;
}

std::string Atlas::VulkanRenderingBackend::getApplicationName() {
	return mApplicationName;
}

void Atlas::VulkanRenderingBackend::setFenceTimeout(uint64_t lengthInNS) {
	mFencesTimeoutNS = lengthInNS;
}

void Atlas::VulkanRenderingBackend::setNextImageTimeout(uint64_t lengthInNS) {
	mNextImageTimeoutNS = lengthInNS;
}

Atlas::FrameData& Atlas::VulkanRenderingBackend::getCurrentFrame()
{
	return mFrameDataArray[mCurrentFrameNumber % FRAME_OVERLAP];
}

void Atlas::VulkanRenderingBackend::initDescriptors()
{
	//create a descriptor pool that will hold 10 sets with 1 image each
	std::vector<DescriptorAllocator::PoolSizeRatio> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 }
	};

	mGlobalDescriptorAllocator.init_pool(mDevice, 10, sizes);

	//make the descriptor set layout for our compute draw
	{
		DescriptorLayoutBuilder builder;
		builder.addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		mDrawImageDescriptorLayout = builder.build(mDevice, VK_SHADER_STAGE_COMPUTE_BIT);
	}

	// other code
	//allocate a descriptor set for our draw image
	mDrawImageDescriptors = mGlobalDescriptorAllocator.allocate(mDevice, mDrawImageDescriptorLayout);

	VkDescriptorImageInfo imgInfo{};
	imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imgInfo.imageView = mDrawImage.imageView;

	VkWriteDescriptorSet drawImageWrite = {};
	drawImageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	drawImageWrite.pNext = nullptr;

	drawImageWrite.dstBinding = 0;
	drawImageWrite.dstSet = mDrawImageDescriptors;
	drawImageWrite.descriptorCount = 1;
	drawImageWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	drawImageWrite.pImageInfo = &imgInfo;

	vkUpdateDescriptorSets(mDevice, 1, &drawImageWrite, 0, nullptr);

	//make sure both the descriptor allocator and the new layout get cleaned up properly
	mMainDeletionQueue.push([&]() {
		mGlobalDescriptorAllocator.destroy_pool(mDevice);

		vkDestroyDescriptorSetLayout(mDevice, mDrawImageDescriptorLayout, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initPipelines()
{
	initBackgroundPipelines();
	//initTrianglePipeline();
	initMeshPipeline();
}

void Atlas::VulkanRenderingBackend::initBackgroundPipelines()
{
	//Device cDeviceHandle = mDevice;

	VkPipelineLayoutCreateInfo computeLayout{};
	computeLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	computeLayout.pNext = nullptr;
	computeLayout.pSetLayouts = &mDrawImageDescriptorLayout;
	computeLayout.setLayoutCount = 1;
	
	VkPushConstantRange pushConstant{};
	pushConstant.offset = 0;
	pushConstant.size = sizeof(ComputePushConstants);
	pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

	computeLayout.pPushConstantRanges = &pushConstant;
	computeLayout.pushConstantRangeCount = 1;

	vkCreatePipelineLayout(mDevice, &computeLayout, nullptr, &mGradientPipelineLayout);
	
	//layout code
	VkShaderModule computeDrawShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/gradient.comp.spv", mDevice, &computeDrawShader))
	{
		std::cout << "Error when building the compute shader \n";
	}

	VkShaderModule skyShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/sky.comp.spv", mDevice, &skyShader))
	{
		std::cout << "Error when building the compute shader \n";
	}
	
	VkPipelineShaderStageCreateInfo stageinfo{};
	stageinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageinfo.pNext = nullptr;
	stageinfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	stageinfo.module = computeDrawShader;
	stageinfo.pName = "main";
	
	VkComputePipelineCreateInfo computePipelineCreateInfo{};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = nullptr;
	computePipelineCreateInfo.layout = mGradientPipelineLayout;
	computePipelineCreateInfo.stage = stageinfo;

	gradient.layout = mGradientPipelineLayout;
	gradient.name = "gradient";
	gradient.data = {};

	//default colors
	gradient.data.data1.data = glm::vec4(1, 0, 0, 1);
	gradient.data.data1.name = "Gradient color1 (r, g, b, a)";

	gradient.data.data2.data = glm::vec4(0, 0, 1, 1);
	gradient.data.data2.name = "Gradient color2 (r, g, b, a)";

	vkCreateComputePipelines(mDevice, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &gradient.pipeline);

	//change the shader module only to create the sky shader
	computePipelineCreateInfo.stage.module = skyShader;

	sky.layout = mGradientPipelineLayout;
	sky.name = "sky";
	sky.data = {};

	//default sky parameters
	sky.data.data1.data = glm::vec4(0.1, 0.2, 0.4, 0.97);
	sky.data.data1.name = "Sky color (r, g, b, a)";

	vkCreateComputePipelines(mDevice, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline);

	//add the 2 background effects into the array
	mBackgroundEffects.push_back(gradient);
	mBackgroundEffects.push_back(sky);

	mEffectManager->pushEffect(gradient);
	mEffectManager->pushEffect(sky);

	vkDestroyShaderModule(mDevice, computeDrawShader, nullptr);
	vkDestroyShaderModule(mDevice, skyShader, nullptr);

	mMainDeletionQueue.push([&]() {
		// Why does this throw an exception when closing lmao. I guess its a "task failed successfully" moment
		vkDestroyPipelineLayout(mDevice, mGradientPipelineLayout, nullptr);
		vkDestroyPipeline(mDevice, sky.pipeline, nullptr);
		
		vkDestroyPipeline(mDevice, gradient.pipeline, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initTrianglePipeline()
{
	VkShaderModule triangleFragShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/colored_triangle.frag.spv", mDevice, &triangleFragShader)) {
		ErrorLog("Error when building the triangle fragment shader module");
	}
	else {
		TraceLog("Triangle fragment shader succesfully loaded");
	}

	VkShaderModule triangleVertexShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/colored_triangle.vert.spv", mDevice, &triangleVertexShader)) {
		ErrorLog("Error when building the triangle vertex shader module");
	}
	else {
		TraceLog("Triangle vertex shader succesfully loaded");
	}

	//build the pipeline layout that controls the inputs/outputs of the shader
	//we are not using descriptor sets or other systems yet, so no need to use anything other than empty default
	VkPipelineLayoutCreateInfo pipeline_layout_info = CreatePipelineLayoutCreateInfo();
	
	vkCreatePipelineLayout(mDevice, &pipeline_layout_info, nullptr, &_trianglePipelineLayout);

	PipelineBuilder pipelineBuilder;

	//use the triangle layout we created
	pipelineBuilder.settings.pipelineLayout = _trianglePipelineLayout;
	//connecting the vertex and pixel shaders to the pipeline
	pipelineBuilder.setShaders(triangleVertexShader, triangleFragShader);
	//it will draw triangles
	pipelineBuilder.setInputTopography(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	//filled triangles
	pipelineBuilder.setPolygonMode(VK_POLYGON_MODE_FILL);
	//no backface culling
	pipelineBuilder.setCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
	//no multisampling
	pipelineBuilder.setMultisamplingNone();
	//no blending
	pipelineBuilder.disableBlending();
	//no depth testing
	pipelineBuilder.disableDepthTest();
	
	//connect the image format we will draw into, from draw image
	pipelineBuilder.setColorAttachmentFormat(mDrawImage.imageFormat);
	pipelineBuilder.setDepthFormat(mDepthImage.imageFormat);

	//finally build the pipeline
	_trianglePipeline = pipelineBuilder.buildPipeline(mDevice);

	//clean structures
	vkDestroyShaderModule(mDevice, triangleFragShader, nullptr);
	vkDestroyShaderModule(mDevice, triangleVertexShader, nullptr);

	mMainDeletionQueue.push([&]() {
		vkDestroyPipelineLayout(mDevice, _trianglePipelineLayout, nullptr);
		vkDestroyPipeline(mDevice, _trianglePipeline, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initMeshPipeline()
{
	VkShaderModule triangleFragShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/colored_triangle.frag.spv", mDevice, &triangleFragShader)) {
		ErrorLog("Error when building the triangle fragment shader module");
	}
	else {
		InfoLog("Triangle fragment shader succesfully loaded");
	}

	// THIS GOD DAMN SHADER FILE HAD SOMEHOW BROKEN THE ENTIRE DRIVER. DO NOT GOD DAMN TOUCH THIS FILE NAME
	VkShaderModule triangleVertexShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/colored_triangle_mesh.vert.spv", mDevice, &triangleVertexShader)) {
		ErrorLog("Error when building the triangle vertex shader module");
	}
	else {
		InfoLog("Triangle vertex shader succesfully loaded");
	}

	VkPushConstantRange bufferRange{};
	bufferRange.offset = 0;
	bufferRange.size = sizeof(GPUDrawPushConstants);
	bufferRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPipelineLayoutCreateInfo pipeline_layout_info = CreatePipelineLayoutCreateInfo();
	pipeline_layout_info.pPushConstantRanges = &bufferRange;
	pipeline_layout_info.pushConstantRangeCount = 1;

	vkCreatePipelineLayout(mDevice, &pipeline_layout_info, nullptr, &_meshPipelineLayout);

	PipelineBuilder pipelineBuilder;

	//use the triangle layout we created
	pipelineBuilder.settings.pipelineLayout = _meshPipelineLayout;

	//connecting the vertex and pixel shaders to the pipeline
	pipelineBuilder.setShaders(triangleVertexShader, triangleFragShader);

	//it will draw triangles
	pipelineBuilder.setInputTopography(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	//filled triangles
	pipelineBuilder.setPolygonMode(VK_POLYGON_MODE_FILL);

	//no backface culling
	pipelineBuilder.setCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);

	//no multisampling
	pipelineBuilder.setMultisamplingNone();

	//no blending
	pipelineBuilder.enableBlendingAdditive();

	pipelineBuilder.enableDepthTest(true, VK_COMPARE_OP_GREATER_OR_EQUAL);

	//connect the image format we will draw into, from draw image
	pipelineBuilder.setColorAttachmentFormat(mDrawImage.imageFormat);
	pipelineBuilder.setDepthFormat(mDepthImage.imageFormat);



	//finally build the pipeline
	_meshPipeline = pipelineBuilder.buildPipeline(mDevice);

	//clean structures
	vkDestroyShaderModule(mDevice, triangleFragShader, nullptr);
	vkDestroyShaderModule(mDevice, triangleVertexShader, nullptr);

	mMainDeletionQueue.push([&]() {
		vkDestroyPipelineLayout(mDevice, _meshPipelineLayout, nullptr);
		vkDestroyPipeline(mDevice, _meshPipeline, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initVMAAllocator(vkb::Instance const& cVkBootstrapInstanceRef)
{
	// initialize the memory allocator (this can be a function)
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = mPhysicalDevice.getVkbHandle();
	allocatorInfo.device = mDevice;
	allocatorInfo.instance = cVkBootstrapInstanceRef;
	allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	vmaCreateAllocator(&allocatorInfo, &mVMAAllocator);
	
	mMainDeletionQueue.push([&]() {
		vmaDestroyAllocator(mVMAAllocator);
	});
}

void Atlas::VulkanRenderingBackend::initCommands()
{
	//create a command pool for commands submitted to the graphics queue.
	//we also want the pool to allow for resetting of individual command buffers
	VkCommandPoolCreateInfo commandPoolInfo = CreateCommandPoolCreateInfo(mGraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	for (int i = 0; i < FRAME_OVERLAP; i++) {

		vkCreateCommandPool(mDevice, &commandPoolInfo, nullptr, &mFrameDataArray.at(i).commandPool);

		// allocate the default command buffer that we will use for rendering
		VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(mFrameDataArray.at(i).commandPool, 1);

		vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &mFrameDataArray.at(i).mainCommandBuffer);
	}

	vkCreateCommandPool(mDevice, &commandPoolInfo, nullptr, &mImmediateSubmitInfo.commandPool);

	// allocate the default command buffer that we will use for rendering
	VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(mImmediateSubmitInfo.commandPool, 1);

	vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &mImmediateSubmitInfo.commandBuffer);

	mMainDeletionQueue.push([=]() {
		vkDestroyCommandPool(mDevice, mImmediateSubmitInfo.commandPool, nullptr);
	});
	
	//// allocate the command buffer for immediate submits
	//VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(mImmediateSubmitInfo.commandPool, 1);

	//vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &mImmediateSubmitInfo.commandBuffer);

	//mMainDeletionQueue.push([=]() {
	//	vkDestroyCommandPool(mDevice, mImmediateSubmitInfo.commandPool, nullptr);
	//});

	////create syncronization structures
	////one fence to control when the gpu has finished rendering the frame,
	////and 2 semaphores to syncronize rendering with swapchain
	////we want the fence to start signalled so we can wait on it on the first frame
	VkFenceCreateInfo fenceCreateInfo = CreateFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	VkSemaphoreCreateInfo semaphoreCreateInfo = CreateSemaphoreCreateInfo();
	
	for (int i = 0; i < FRAME_OVERLAP; i++) {
		vkCreateFence(mDevice, &fenceCreateInfo, nullptr, &mFrameDataArray[i].renderFence);

		vkCreateSemaphore(mDevice, &semaphoreCreateInfo, nullptr, &mFrameDataArray[i].swapchainSemaphore);
		vkCreateSemaphore(mDevice, &semaphoreCreateInfo, nullptr, &mFrameDataArray[i].renderSemaphore);
	}

	vkCreateFence(mDevice, &fenceCreateInfo, nullptr, &mImmediateSubmitInfo.fence);
	mMainDeletionQueue.push([=]() { vkDestroyFence(mDevice, mImmediateSubmitInfo.fence, nullptr); });
}

void Atlas::VulkanRenderingBackend::initIMGUI(GameWindow* gameWindow)
{
	// 1: create descriptor pool for IMGUI
	//  the size of the pool is very oversize, but it's copied from imgui demo
	//  itself.
	VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	vkCreateDescriptorPool(mDevice, &pool_info, nullptr, &imguiPool);

	// 2: initialize imgui library

	// this initializes the core structures of imgui
	ImGui::CreateContext();

	// this initializes imgui for SDL
	ImGui_ImplSDL2_InitForVulkan(gameWindow->getWindowHandle());

	// this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = mInstance.getInstance();
	init_info.PhysicalDevice = mPhysicalDevice.getVkHandle();
	init_info.Device = mDevice;
	init_info.Queue = mGraphicsQueue;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.UseDynamicRendering = true;

	//dynamic rendering parameters for imgui to use
	init_info.PipelineRenderingCreateInfo = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
	init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &mSwapchainImageFormat;


	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&init_info);
	
	mRenderPassesManager.addRenderPass(std::make_shared<IMGUIRenderPass>());

	//ImGui_ImplVulkan_CreateFontsTexture();

	// add the destroy the imgui created structures
	mMainDeletionQueue.push([=]() {
		ImGui_ImplVulkan_Shutdown();
		vkDestroyDescriptorPool(mDevice, imguiPool, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::createSwapchain(uint32_t width, uint32_t height)
{
	vkb::SwapchainBuilder swapchainBuilder{ mPhysicalDevice.getVkbHandle(), mDevice, mSurface};
	
	mSwapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	swapchainBuilder.set_desired_format(VkSurfaceFormatKHR{ .format = mSwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });
	swapchainBuilder.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR);
	swapchainBuilder.set_desired_extent(width, height);
	swapchainBuilder.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT);

	auto swapchainBuildResult = swapchainBuilder.build();

	vkb::Swapchain vkbSwapchain = swapchainBuildResult.value();
		////.use_default_format_selection()
		//.set_desired_format(VkSurfaceFormatKHR{ .format = mSwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		////use vsync present mode
		//.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		//.set_desired_extent(width, height)
		//.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		//.build()
		//.value();

	mSwapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images
	mSwapchain = vkbSwapchain.swapchain;
	mSwapchainImages = vkbSwapchain.get_images().value();
	mSwapchainImageViews = vkbSwapchain.get_image_views().value();
}


void Atlas::VulkanRenderingBackend::destroySwapchain()
{
	vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < mSwapchainImageViews.size(); i++) {

		vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
	}
}

GPUMeshBuffers Atlas::VulkanRenderingBackend::UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices)
{
	const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
	const size_t indexBufferSize = indices.size() * sizeof(uint32_t);

	GPUMeshBuffers newSurface;

	//create vertex buffer
	newSurface.vertexBuffer = AllocatedBuffer(mVMAAllocator, vertexBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY);
	
	//find the adress of the vertex buffer
	VkBufferDeviceAddressInfo deviceAdressInfo{ .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,.buffer = newSurface.vertexBuffer.getBuffer()};
	newSurface.vertexBufferAddress = vkGetBufferDeviceAddress(mDevice, &deviceAdressInfo);

	//create index buffer
	newSurface.indexBuffer = AllocatedBuffer(mVMAAllocator, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VMA_MEMORY_USAGE_GPU_ONLY);

	AllocatedBuffer staging = AllocatedBuffer(mVMAAllocator, vertexBufferSize + indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);

	void* data = staging.getAllocation()->GetMappedData();

	// copy vertex buffer
	memcpy(data, vertices.data(), vertexBufferSize);

	// copy index buffer
	memcpy((char*)data + vertexBufferSize, indices.data(), indexBufferSize);

	ImmediateSubmit([&](VkCommandBuffer cmd) {
		VkBufferCopy vertexCopy{ 0 };
		vertexCopy.dstOffset = 0;
		vertexCopy.srcOffset = 0;
		vertexCopy.size = vertexBufferSize;

		vkCmdCopyBuffer(cmd, staging.getBuffer(), newSurface.vertexBuffer.getBuffer(), 1, &vertexCopy);

		VkBufferCopy indexCopy{ 0 };
		indexCopy.dstOffset = 0;
		indexCopy.srcOffset = vertexBufferSize;
		indexCopy.size = indexBufferSize;

		vkCmdCopyBuffer(cmd, staging.getBuffer(), newSurface.indexBuffer.getBuffer(), 1, &indexCopy);
	});

	staging.destroy(mVMAAllocator);

	return newSurface;
}

void Atlas::VulkanRenderingBackend::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function)
{
	vkResetFences(mDevice, 1, &mImmediateSubmitInfo.fence);
	vkResetCommandBuffer(mImmediateSubmitInfo.commandBuffer, 0);

	VkCommandBuffer cmd = mImmediateSubmitInfo.commandBuffer;
	VkCommandBufferBeginInfo cmdBeginInfo = CreateCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkBeginCommandBuffer(cmd, &cmdBeginInfo);

	function(cmd);

	vkEndCommandBuffer(cmd);

	VkCommandBufferSubmitInfo cmdinfo = SubmitCommandBufferInfo(cmd);
	VkSubmitInfo2 submit = SubmitInfo(&cmdinfo, nullptr, nullptr);

	// submit command buffer to the queue and execute it.
	//  _renderFence will now block until the graphic commands finish execution

	// For whatever reason, this is causing errors within Vulkan. The output says that the command buffer in 
	// the submitInfo is using a VkNonDispatchableHandle 
	vkQueueSubmit2(mGraphicsQueue, 1, &submit, mImmediateSubmitInfo.fence);

	constexpr uint64_t cTimeout = 9999999999;

	vkWaitForFences(mDevice, 1, &mImmediateSubmitInfo.fence, true, cTimeout);
}


Atlas::VulkanRenderingBackend::VulkanRenderingBackend(const ApplicationInfo& appInfo, GameWindow* gameWindow) : mApplicationName(appInfo.name), mAppInfo(appInfo) {
}

void Atlas::VulkanRenderingBackend::init(GameWindow* gameWindow)
{

	setLoadedRenderingBackend(this);

	InfoLog("Attempting to initialize Vulkan");

	//// Just make sure everything is in order before Vulkan gets initialized
	ATLAS_ASSERT(gameWindow != nullptr, "Window must be set up prior to Vulkan init");

	if (canInitialize(gameWindow) == false) {
		//std::cout << "Window must be set up and open prior to Vulkan init" << std::endl;
		ErrorLog("Window must be set up and open prior to Vulkan init!");

		return;
	}

	ApplicationInfo appInfo = ApplicationInfo();
	const bool cbEnableValidationLayers = isErrorCheckingEnabled();
	const APIVersion cApiVersion = appInfo.vulkanVersion;

	//// TODO: Figure out how to stop OBS from overriding the API version.
	//// The issue is that, if OBS is installed, it will override the API 
	//// version, even if a higher version is set. This is a problem because
	//// it can cause the API version to be set to a lower version than what
	//// we want. Currently, it is not a major problem, but it is something
	//// to keep in mind.

	std::string vulkanInitParametersString
		= std::format("->Application Name: {}\n->Vulkan API Version: {}.{}.{}\n->Enable Validation Layers: {}\n", mApplicationName, cApiVersion.major, cApiVersion.minor, cApiVersion.patch, cbEnableValidationLayers);

	InfoLog("Initializing Vulkan instance:\n" + vulkanInitParametersString);

	//mInstance.setVersion(cApiVersion);
	//mInstance.setApplicationName(mApplicationName);
	//mInstance.setEnableValidationLayers(cbEnableValidationLayers);

	mInstance = VulkanInstanceWrapper(VulkanInstanceWrapper::InitConfiguration());
	mInstance.init();
	vkb::Instance const& cVkBootstrapInstanceRef = mInstance.getVulkanBootstrapInstance();

	// Device init
	InfoLog("Initializing Vulkan device");

	// If vulkan is being used with SDL2, this has to be done here.
#ifdef ATLAS_USE_SDL2
	InfoLog("Initializing Vulkan SDL2 surface");

	mSurface = VK_NULL_HANDLE;
	// directly passing the window cast to avoid memory issues.
	SDL_Vulkan_CreateSurface(gameWindow->getWindowHandle(), cVkBootstrapInstanceRef.instance, &mSurface);
#endif // ATLAS_USE_SDL2

	initPhysicalDevice();

	//create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ mPhysicalDevice.getVkbHandle() };

	mDevice = deviceBuilder.build().value();

	//mDevice = Device(vkbDevice.device);
	//mDevice.setAsMainHandle();

	mPhysicalDevice.getVkbHandle();

	// Get the VkDevice handle used in the rest of a vulkan application
	//mDevice = vkbDevice.device;
	//mGPUDevice = physicalDevice.physical_device;
	//vkb::Device& vkbDeviceRef = mDevice.getVkbHandle();
	//mGraphicsQueue = mDevice.getVkbHandle().get_queue(vkb::QueueType::graphics).value();
	//mGraphicsQueueFamily = mDevice.getVkbHandle().get_queue_index(vkb::QueueType::graphics).value();

	mGraphicsQueue = mDevice.get_queue(vkb::QueueType::graphics).value();
	mGraphicsQueueFamily = mDevice.get_queue_index(vkb::QueueType::graphics).value();

	mEffectManager = std::make_shared<EffectManager>();

	

	initVMAAllocator(cVkBootstrapInstanceRef);

	initSwapchain(gameWindow);

	initCommands();

	initDescriptors();

	initPipelines();

	initIMGUI(gameWindow);

	initDefaultData();

	mIsInitialized = true;
}

void Atlas::VulkanRenderingBackend::initDefaultData()
{
	//std::array<Vertex, 4> rect_vertices;

	//rect_vertices[0].position = { 0.5,-0.5, 0 };
	//rect_vertices[1].position = { 0.5,0.5, 0 };
	//rect_vertices[2].position = { -0.5,-0.5, 0 };
	//rect_vertices[3].position = { -0.5,0.5, 0 };

	//rect_vertices[0].color = { 1,1, 0,1 };
	//rect_vertices[1].color = { 0.5,0.5,0.5 ,1 };
	//rect_vertices[2].color = { 1,0, 0,1 };
	//rect_vertices[3].color = { 0,1, 0,1 };

	//std::array<uint32_t, 6> rect_indices;

	//rect_indices[0] = 0;
	//rect_indices[1] = 1;
	//rect_indices[2] = 2;

	//rect_indices[3] = 2;
	//rect_indices[4] = 1;
	//rect_indices[5] = 3;

	//rectangle = UploadMesh(rect_indices, rect_vertices);

	//delete the rectangle data on engine shutdown
	mMainDeletionQueue.push([&]() {
		//rectangle.indexBuffer.destroy();
		//rectangle.vertexBuffer.destroy();
	});
	
	//testMeshes = loadGltfMeshes(this, "C:\\Dev\\Techstorm-v5\\temp\\resources\\models\\old_car_new.glb").value();
	testMeshes = loadGltfMeshes(this, "C:\\Dev\\Techstorm-v5\\basicmesh.glb").value();
}

void Atlas::VulkanRenderingBackend::initPhysicalDevice()
{
	// Vulkan 1.3 features
	//--------------------
	constexpr bool cbEnableDynamicRendering = true;
	constexpr bool cbEnableSynchronization2 = true;

	VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features.dynamicRendering = cbEnableDynamicRendering;
	features.synchronization2 = cbEnableSynchronization2;


	// Vulkan 1.2 features
	//--------------------
	
	constexpr bool cbEnableBufferDeviceAddress = true;
	constexpr bool cbEnableDescriptorIndexing = true;
	
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = cbEnableBufferDeviceAddress;
	features12.descriptorIndexing = cbEnableDescriptorIndexing;

	PhysicalDeviceSelectionConstraints constraints{};

	constraints.minimumAPIVersion = mAppInfo.vulkanVersion;
	constraints.physicalDeviceFeatures = { features, features12 };
	constraints.surface = mSurface;

	mPhysicalDevice = PhysicalDevice();
	mPhysicalDevice.init(mPhysicalDevice.selectDevice(mInstance, constraints));
	// This static cast is required since the result of mPhysicalDevice.getName() is a std::string_view
	const std::string cDeviceName = static_cast<std::string>(mPhysicalDevice.getName());

	InfoLog("Selected GPU: " + cDeviceName);
}

void Atlas::VulkanRenderingBackend::initSwapchain(GameWindow* gameWindow)
{
	// swapchain init

	const WindowDescription activeWindowDescription = gameWindow->getWindowDescription();
	const int cWindowWidth = activeWindowDescription.windowRect.getWidth();
	const int cWindowHeight = activeWindowDescription.windowRect.getHeight();

	createSwapchain(cWindowWidth, cWindowHeight);

	SDL_Rect displayBounds;
	SDL_GetDisplayBounds(0, &displayBounds);
	
	//draw image size will match the window
	VkExtent3D drawImageExtent = {
		displayBounds.w,
		displayBounds.h,
		1
	};


	GraphicsAllocationInfo drawImageAllocation = {};
	drawImageAllocation.allocator = mVMAAllocator;
	drawImageAllocation.createInfo = { };
	drawImageAllocation.createInfo->usage = VMA_MEMORY_USAGE_GPU_ONLY;
	drawImageAllocation.createInfo->requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//hardcoding the draw format to 32 bit float
	mDrawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	mDrawImage.imageExtent = drawImageExtent;

	VkImageUsageFlags drawImageUsages{};
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VkImageCreateInfo rimg_info = CreateImageCreateInfo(mDrawImage.imageFormat, drawImageUsages, drawImageExtent);

	//for the draw image, we want to allocate it from gpu local memory
	VmaAllocationCreateInfo rimg_allocinfo = {};
	rimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	rimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//allocate and create the image
	vmaCreateImage(mVMAAllocator, &rimg_info, &rimg_allocinfo, &mDrawImage.image, &mDrawImage.allocation, nullptr);

	//build a image-view for the draw image to use for rendering
	VkImageViewCreateInfo rview_info = CreateImageViewCreateInfo(mDrawImage.imageFormat, mDrawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);
	
	vkCreateImageView(mDevice, &rview_info, nullptr, &mDrawImage.imageView);
	
	// Depth image
	mDepthImage.imageFormat = VK_FORMAT_D32_SFLOAT;
	mDepthImage.imageExtent = drawImageExtent;
	VkImageUsageFlags depthImageUsages{};
	depthImageUsages |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	VkImageCreateInfo depthImageInfo = CreateImageCreateInfo(mDepthImage.imageFormat, depthImageUsages, drawImageExtent);

	//allocate and create the image
	VkResult depthImageResult = vmaCreateImage(mVMAAllocator, &depthImageInfo, &rimg_allocinfo, &mDepthImage.image, &mDepthImage.allocation, nullptr);

	//build a image-view for the draw image to use for rendering
	VkImageViewCreateInfo dview_info = CreateImageViewCreateInfo(mDepthImage.imageFormat, mDepthImage.image, VK_IMAGE_ASPECT_DEPTH_BIT);

	VkResult depthImageViewResult = vkCreateImageView(mDevice, &dview_info, nullptr, &mDepthImage.imageView);

	//add to deletion queues
	mMainDeletionQueue.push([=]() {
		vkDestroyImageView(mDevice, mDrawImage.imageView, nullptr);
		vmaDestroyImage(mVMAAllocator, mDrawImage.image, mDrawImage.allocation);

		vkDestroyImageView(mDevice, mDepthImage.imageView, nullptr);
		vmaDestroyImage(mVMAAllocator, mDepthImage.image, mDepthImage.allocation);
	});
}

void Atlas::VulkanRenderingBackend::initSyncStructures()
{
}

void Atlas::VulkanRenderingBackend::resetFences(const uint32_t cFenceCount, FrameData& currentFrame)
{
	vkWaitForFences(mDevice, cFenceCount, &currentFrame.renderFence, true, mFencesTimeoutNS);

	currentFrame.deletionQueue.flush();
	
	vkResetFences(mDevice, cFenceCount, &currentFrame.renderFence);
}

void Atlas::VulkanRenderingBackend::beginDrawingMode()
{

	FrameData& currentFrame = getCurrentFrame();

	// BUG: Occasionally, this will hang on the vkWaitForFences call with a VK_ERROR_DEVICE_LOST error. Not sure why.
	// From what I am reading, it says that it can be due to drivers. Furthermore, according to a forum post, this 
	// seems to be an issue with RTX cards, or something like that.
	if (const VkResult cWaitResult = vkWaitForFences(mDevice, 1, &currentFrame.renderFence, false, mFencesTimeoutNS); cWaitResult != VK_SUCCESS) {

		if (cWaitResult == VK_TIMEOUT) {
			ErrorLog("Waiting for fence timed out!");
		}
		else if (cWaitResult == VK_ERROR_DEVICE_LOST) {
			// TODO: Figure out why this happens
			throw AException("Failed to wait for fence because the device was lost!");
		}
		else {
			throw AException("failed to wait for fence for an unknown reason!");
		}
	}
	
	currentFrame.deletionQueue.flush();
	
	//request image from the swapchain
	uint32_t swapchainImageIndex;
	VkResult result = vkAcquireNextImageKHR(mDevice, mSwapchain, mNextImageTimeoutNS, currentFrame.swapchainSemaphore, nullptr, &swapchainImageIndex);

	if (result != VK_SUCCESS)
	{
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			mbResizeRequested = true;
			return;
		}
		else {
			throw AException("failed to acquire swap chain image!");
		}

	}

	mDrawExtent.height = std::min(mSwapchainExtent.height, mDrawImage.imageExtent.height) * mRenderScale;
	mDrawExtent.width = std::min(mSwapchainExtent.width, mDrawImage.imageExtent.width) * mRenderScale;

	// reset command buffer
	mCurrentDrawData.cmdResetFlags = 0;

	//naming it cmd for shorter writing

	vkResetFences(mDevice, 1, &currentFrame.renderFence);

	// now that we are sure that the commands finished executing, we can safely
	// reset the command buffer to begin recording again.
	vkResetCommandBuffer(currentFrame.mainCommandBuffer, mCurrentDrawData.cmdResetFlags);

	mCurrentDrawData.cmd = currentFrame.mainCommandBuffer;

	//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
	mCurrentDrawData.cmdBeginInfo = CreateCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	mCurrentDrawData.computeEffects = mEffectManager;

	mCurrentDrawData.swapchain = mSwapchain;
	mCurrentDrawData.swapchainImages = mSwapchainImages;
	mCurrentDrawData.swapchainImageViews = mSwapchainImageViews;
	mCurrentDrawData.swapchainImageIndex = swapchainImageIndex;

	mCurrentDrawData.currentSwapchainImageView = mSwapchainImageViews[mCurrentDrawData.swapchainImageIndex];
	mCurrentDrawData.swapchainExtent = mSwapchainExtent;


	vkBeginCommandBuffer(mCurrentDrawData.cmd, &mCurrentDrawData.cmdBeginInfo);
}

void Atlas::VulkanRenderingBackend::draw()
{
	// Stop drawing if a resize was requested so we can resize the swapchain
	if (mbResizeRequested)
	{
		return;
	}

	mRenderPassesManager.beginDrawingRenderPasses(mCurrentDrawData.cmd, mCurrentDrawData);

	FrameData& currentFrame = getCurrentFrame();
	// transition our main draw image into general layout so we can write into it
	// we will overwrite it all so we dont care about what was the older layout
	TransitionImage(mCurrentDrawData.cmd, mDrawImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
	
	drawBackground(mCurrentDrawData.cmd);

	//transition the draw image and the swapchain image into their correct transfer layouts
	TransitionImage(mCurrentDrawData.cmd, mDrawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	TransitionImage(mCurrentDrawData.cmd, mDepthImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
	drawGeometry(mCurrentDrawData.cmd);

	TransitionImage(mCurrentDrawData.cmd, mDrawImage.image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	TransitionImage(mCurrentDrawData.cmd, mSwapchainImages[mCurrentDrawData.swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// execute a copy from the draw image into the swapchain
	CopyImageToImage(mCurrentDrawData.cmd, mDrawImage.image, mSwapchainImages[mCurrentDrawData.swapchainImageIndex], mDrawExtent, mSwapchainExtent);

	//mRenderPassesManager.drawRenderPasses(mCurrentDrawData.cmd, mCurrentDrawData);

	drawIMGUI(mCurrentDrawData.cmd, mSwapchainImageViews[mCurrentDrawData.swapchainImageIndex]);
	
	// set swapchain image layout to Present so we can show it on the screen
	TransitionImage(mCurrentDrawData.cmd, mSwapchainImages[mCurrentDrawData.swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}

void Atlas::VulkanRenderingBackend::endDrawingMode()
{
	// Stop drawing if a resize was requested so we can resize the swapchain
	if (mbResizeRequested)
	{
		return;
	}

	FrameData& currentFrame = getCurrentFrame();

	//end render pass
	mRenderPassesManager.endDrawingRenderPasses(mCurrentDrawData.cmd, mCurrentDrawData);

	//finalize the command buffer (we can no longer add commands, but it can now be executed)
	vkEndCommandBuffer(mCurrentDrawData.cmd);

	//prepare the submission to the queue. 
	//we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
	//we will signal the _renderSemaphore, to signal that rendering has finished

	VkCommandBufferSubmitInfo cmdinfo = SubmitCommandBufferInfo(mCurrentDrawData.cmd);

	VkSemaphoreSubmitInfo waitInfo = SubmitSemaphoreInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, currentFrame.swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo = SubmitSemaphoreInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, currentFrame.renderSemaphore);

	VkSubmitInfo2 submit = SubmitInfo(&cmdinfo, &signalInfo, &waitInfo);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	vkQueueSubmit2(mGraphicsQueue, 1, &submit, currentFrame.renderFence);

	// prepare present
	// this will put the image we just rendered to into the visible window.
	// we want to wait on the _renderSemaphore for that, 
	// as its necessary that drawing commands have finished before the image is displayed to the user
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.pSwapchains = &mSwapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &currentFrame.renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;
	
	presentInfo.pImageIndices = &mCurrentDrawData.swapchainImageIndex;

	VkResult presentResult = vkQueuePresentKHR(mGraphicsQueue, &presentInfo);
	if (presentResult == VK_ERROR_OUT_OF_DATE_KHR) {
		mbResizeRequested = true;
	}

	mFrameTime.update();


	//increase the number of frames drawn
	mCurrentFrameNumber++;

	// reset the current draw data to default values
	mCurrentDrawData = {};

	
}

void Atlas::VulkanRenderingBackend::drawIMGUI(VkCommandBuffer cmd, VkImageView targetImageView)
{
	VkRenderingAttachmentInfo colorAttachment = CreateAttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	VkRenderingInfo renderInfo = CreateRenderingInfo(mSwapchainExtent, &colorAttachment, nullptr);

	vkCmdBeginRendering(cmd, &renderInfo);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

	vkCmdEndRendering(cmd);
}

void Atlas::VulkanRenderingBackend::drawBackground(VkCommandBuffer cmd)
{
	ComputeEffect& effect = mEffectManager->getCurrentEffect();

	// bind the background compute pipeline
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, effect.pipeline);

	// bind the descriptor set containing the draw image for the compute pipeline
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, mGradientPipelineLayout, 0, 1, &mDrawImageDescriptors, 0, nullptr);

	vkCmdPushConstants(cmd, mGradientPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ComputePushConstants), &effect.data);
	// execute the compute pipeline dispatch. We are using 16x16 workgroup size so we need to divide by it
	vkCmdDispatch(cmd, std::ceil(mDrawExtent.width / 16.0), std::ceil(mDrawExtent.height / 16.0), 1);
}

void Atlas::VulkanRenderingBackend::drawGeometry(VkCommandBuffer cmd)
{

	//begin a render pass  connected to our draw image
	VkRenderingAttachmentInfo colorAttachment = CreateAttachmentInfo(mDrawImage.imageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	VkRenderingAttachmentInfo depthAttachment = CreateDepthAttachmentInfo(mDepthImage.imageView, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

	VkRenderingInfo renderInfo = CreateRenderingInfo(mDrawExtent, &colorAttachment, &depthAttachment);
	vkCmdBeginRendering(cmd, &renderInfo);

	//vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _trianglePipeline);


	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _meshPipeline);
	//set dynamic viewport and scissor	
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = mDrawExtent.width;
	viewport.height = mDrawExtent.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	vkCmdSetViewport(cmd, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = viewport.width;
	scissor.extent.height = viewport.height;

	vkCmdSetScissor(cmd, 0, 1, &scissor);

	GPUDrawPushConstants push_constants{};


	glm::mat4 view = glm::translate(sCameraPosition);
	// camera projection
	glm::mat4 projection = glm::perspective(glm::radians(sFOVY), (float)mDrawExtent.width / (float)mDrawExtent.height, sClipFar, sClipNear);

	// invert the Y direction on projection matrix so that we are more similar
	// to opengl and gltf axis
	projection[1][1] *= -1;
	push_constants.worldMatrix = projection * view;
	
	//vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);

	int i = sModelId;

	push_constants.vertexBuffer = testMeshes[i]->meshBuffers.vertexBufferAddress;

	vkCmdPushConstants(cmd, _meshPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GPUDrawPushConstants), &push_constants);
	vkCmdBindIndexBuffer(cmd, testMeshes[i]->meshBuffers.indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(cmd, testMeshes[i]->surfaces[0].count, 1, testMeshes[i]->surfaces[0].startIndex, 0, 0);

	vkCmdEndRendering(cmd);
}

void Atlas::VulkanRenderingBackend::shutdown()
{

	// No need to worry about cleaning up if not initialized, hence the check
	if (mIsInitialized) {

		/*Device mainDevicePtr = mDevice;*/
		
		vkDeviceWaitIdle(mDevice);
		//mainDevicePtr.waitIdle();

		for (int i = 0; i < FRAME_OVERLAP; i++) {
			vkDestroyCommandPool(mDevice, mFrameDataArray.at(i).commandPool, nullptr);

			//destroy sync objects
			vkDestroyFence(mDevice, mFrameDataArray.at(i).renderFence, nullptr);
			vkDestroySemaphore(mDevice, mFrameDataArray.at(i).renderSemaphore, nullptr);
			vkDestroySemaphore(mDevice, mFrameDataArray.at(i).swapchainSemaphore, nullptr);

			mFrameDataArray.at(i).deletionQueue.flush();
		}

		for (auto& mesh : testMeshes) {
			mesh->meshBuffers.indexBuffer.destroy();
			mesh->meshBuffers.vertexBuffer.destroy();
		}

		mMainDeletionQueue.flush();

		destroySwapchain();

		vkDestroySurfaceKHR(mInstance.getInstance(), mSurface, nullptr);

		vkDestroyDevice(mDevice, nullptr);

		mInstance.shutdown();

		//vkb::destroy_debug_utils_messenger(mVulkanInstance, mDebugMessenger);
		//vkDestroyInstance(mVulkanInstance, nullptr);

		mIsInitialized = false;
	}
}

void Atlas::VulkanRenderingBackend::shouldUseDefaultInstanceBuilder(bool bUseDefaultInstanceBuilder)
{
	mbUseDefaultInstanceBuilder = bUseDefaultInstanceBuilder;
}

bool Atlas::VulkanRenderingBackend::checkValidationLayerSupport() {
	//uint32_t layerCount;
	//vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	//std::vector<VkLayerProperties> availableLayers(layerCount);
	//vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


	//for (const char* layerName : this->mValidationLayers) {
	//	bool layerFound = false;

	//	for (const auto& layerProperties : availableLayers) {
	//		if (strcmp(layerName, layerProperties.layerName) == 0) {
	//			layerFound = true;
	//			break;
	//		}
	//	}

	//	if (!layerFound) {
	//		return false;
	//	}
	//}

	return true;
}

bool Atlas::VulkanRenderingBackend::canInitialize(GameWindow* gameWindow)
{
	return gameWindow->isInit() && gameWindow->isOpen();
}

Atlas::VulkanRenderingBackend& Atlas::getLoadedRenderingBacked()
{
	return *gLoadedVulkanBackend;
}

void Atlas::setLoadedRenderingBackend(VulkanRenderingBackend* backend)
{
	if (gLoadedVulkanBackend != nullptr) {
		gLoadedVulkanBackend->shutdown();

		delete gLoadedVulkanBackend;
	}

	gLoadedVulkanBackend = backend;
}

void Atlas::resetLoadedRenderingBackend()
{
	setLoadedRenderingBackend(nullptr);
}

std::optional<std::vector<std::shared_ptr<MeshAsset>>> Atlas::loadGltfMeshes(VulkanRenderingBackend* engine, std::filesystem::path filePath)
{
	std::cout << "Loading GLTF: " << filePath << std::endl;

	fastgltf::GltfDataBuffer data;
	data.loadFromFile(filePath);

	constexpr auto gltfOptions = fastgltf::Options::LoadGLBBuffers
		| fastgltf::Options::LoadExternalBuffers;

	fastgltf::Asset gltf;
	fastgltf::Parser parser{};

	auto load = parser.loadBinaryGLTF(&data, filePath.parent_path(), gltfOptions);
	if (load) {
		gltf = std::move(load.get());
	}
	else {
		fmt::print("Failed to load glTF: {} \n", fastgltf::to_underlying(load.error()));
		return {};
	}

	std::vector<std::shared_ptr<MeshAsset>> meshes;

	// use the same vectors for all meshes so that the memory doesnt reallocate as
	// often
	std::vector<uint32_t> indices;
	std::vector<Vertex> vertices;
	for (fastgltf::Mesh& mesh : gltf.meshes) {
		MeshAsset newmesh;

		newmesh.name = mesh.name;

		// clear the mesh arrays each mesh, we dont want to merge them by error
		indices.clear();
		vertices.clear();

		for (auto&& p : mesh.primitives) {
			GeoSurface newSurface;
			newSurface.startIndex = (uint32_t)indices.size();
			newSurface.count = (uint32_t)gltf.accessors[p.indicesAccessor.value()].count;

			size_t initial_vtx = vertices.size();

			// load indexes
			{
				fastgltf::Accessor& indexaccessor = gltf.accessors[p.indicesAccessor.value()];
				indices.reserve(indices.size() + indexaccessor.count);

				fastgltf::iterateAccessor<std::uint32_t>(gltf, indexaccessor,
					[&](std::uint32_t idx) {
						indices.push_back(idx + initial_vtx);
					});
			}

			// load vertex positions
			{
				fastgltf::Accessor& posAccessor = gltf.accessors[p.findAttribute("POSITION")->second];
				vertices.resize(vertices.size() + posAccessor.count);

				fastgltf::iterateAccessorWithIndex<glm::vec3>(gltf, posAccessor,
					[&](glm::vec3 v, size_t index) {
						Vertex newvtx;
						newvtx.position = v;
						newvtx.normal = { 1, 0, 0 };
						newvtx.color = glm::vec4{ 1.f };
						newvtx.uv_x = 0;
						newvtx.uv_y = 0;
						vertices[initial_vtx + index] = newvtx;
					});
			}

			// load vertex normals
			auto normals = p.findAttribute("NORMAL");
			if (normals != p.attributes.end()) {

				fastgltf::iterateAccessorWithIndex<glm::vec3>(gltf, gltf.accessors[(*normals).second],
					[&](glm::vec3 v, size_t index) {
						vertices[initial_vtx + index].normal = v;
					});
			}

			// load UVs
			auto uv = p.findAttribute("TEXCOORD_0");
			if (uv != p.attributes.end()) {

				fastgltf::iterateAccessorWithIndex<glm::vec2>(gltf, gltf.accessors[(*uv).second],
					[&](glm::vec2 v, size_t index) {
						vertices[initial_vtx + index].uv_x = v.x;
						vertices[initial_vtx + index].uv_y = v.y;
					});
			}

			// load vertex colors
			auto colors = p.findAttribute("COLOR_0");
			if (colors != p.attributes.end()) {

				fastgltf::iterateAccessorWithIndex<glm::vec4>(gltf, gltf.accessors[(*colors).second],
					[&](glm::vec4 v, size_t index) {
						vertices[initial_vtx + index].color = v;
					});
			}
			newmesh.surfaces.push_back(newSurface);
		}

		// display the vertex normals
		constexpr bool OverrideColors = true;
		if (OverrideColors) {
			for (Vertex& vtx : vertices) {
				vtx.color = glm::vec4(vtx.normal, 1.f);
			}
		}
		newmesh.meshBuffers = engine->UploadMesh(indices, vertices);

		meshes.emplace_back(std::make_shared<MeshAsset>(std::move(newmesh)));
	}

	return meshes;
}

#endif

bool Atlas::ImGuiRenderable::setupElements(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	if (ImGui::Begin("background")) {
		if (ImGui::BeginTabBar("##MyTabBar", ImGuiTabBarFlags_None))
		{
			// Begin the first tab
			if (ImGui::BeginTabItem("Renderer"))
			{
				if (ImGui::BeginTabBar("##RendererTabBar", ImGuiTabBarFlags_None)) {
					if (ImGui::BeginTabItem("General")) {
						ImGui::Text("Current Frame: %i", gLoadedVulkanBackend->mCurrentFrameNumber);
						ImGui::Text("FPS: %f",gLoadedVulkanBackend->mFrameTime.getFPS());
						
						ImGui::SliderFloat("Render Scale", &VulkanRenderingBackend::mRenderScale, 0.3f, 1.f);

						ImGui::SliderInt("Draw Model ID", &sModelId, 0, 2);
						
						if (ImGui::CollapsingHeader("Camera")) {
							ImGui::SliderFloat("FOV", &VulkanRenderingBackend::sFOVY, 0.f, 180.f);

							ImGui::InputFloat("Clip Near", &VulkanRenderingBackend::sClipNear);
							ImGui::InputFloat("Clip Far", &VulkanRenderingBackend::sClipFar);

							ImGui::SliderFloat("Camera Position X", &sCameraPosition.x, -100.f, 100.f);
							ImGui::SliderFloat("Camera Position Y", &sCameraPosition.y, -100.f, 100.f);
							ImGui::SliderFloat("Camera Position Z", &sCameraPosition.z, -100.f, 100.f);
						}

						if (ImGui::CollapsingHeader("Startup info")) {
							ImGui::Text("\nApplication Name: %s", gLoadedVulkanBackend->mApplicationName.c_str());
							ImGui::Text("\nAPI Version: %s", gLoadedVulkanBackend->getAPIVersion().toString().c_str());
							ImGui::Text("\nGPU Name: %s", gLoadedVulkanBackend->mPhysicalDevice.getName().data());

						}
						ImGui::EndTabItem();
					}
					


					if (ImGui::BeginTabItem("Effects")) {
						ComputeEffect& selected = cDrawData.computeEffects->getCurrentEffect();

						ImGui::Text("Effect count: %i", cDrawData.computeEffects->getEffectCount());
						ImGui::Text("Selected effect: %s", selected.name);

						//std::cout << mEffectManager.mCurrentEffectIndex << std::endl;
						//std::cout << mCurrentBackgroundEffect << std::endl;

						//ImGui::Checkbox("Enabled", &this->getInternalValidityValue());

						ImGui::SliderInt("Effect Index", &cDrawData.computeEffects->mCurrentEffectIndex, 0, cDrawData.computeEffects->getEffectCount() - 1);

						ImGui::InputFloat4(selected.data.data1.name.c_str(), (float*)&selected.data.data1);
						ImGui::InputFloat4(selected.data.data2.name.c_str(), (float*)&selected.data.data2);
						ImGui::InputFloat4(selected.data.data3.name.c_str(), (float*)&selected.data.data3);
						ImGui::InputFloat4(selected.data.data4.name.c_str(), (float*)&selected.data.data4);

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				ImGui::EndTabItem(); // End the tab item
			}

			// Begin the second tab
			if (ImGui::BeginTabItem("Tab 2"))
			{
				ImGui::Text("Content of the second tab.");
				ImGui::EndTabItem(); // End the tab item
			}

			ImGui::EndTabBar(); // End the tab bar
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File")) // Check if "File" menu is open
			{
				if (ImGui::MenuItem("New")) {
					// Handle "New" item clicked
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {
					// Handle "Open" item clicked
				}
				ImGui::EndMenu(); // Must be called if BeginMenu returned true
			}
			ImGui::EndMainMenuBar(); // End the main menu bar

		}
	}
	ImGui::End();

	return 0;
}

bool Atlas::ImGuiWindowRenderable::setupElements(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) { 
	return 0;
}

bool Atlas::ImGuiWindowRenderable::beginDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData, EffectManager& computeEffects) {
	return ImGui::Begin(this->getName().data());
}

bool Atlas::ImGuiWindowRenderable::endDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	ImGui::End();
	return 0;
}

void Atlas::IMGUIRenderPass::beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	// imgui new frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	//some imgui UI to test
	//ImGui::ShowDemoWindow();
	
	for (auto& renderable : mIMGUIRenderables) {
		renderable->setupElements(cmd, cDrawData);
	}

	//make imgui calculate internal draw structures
	ImGui::Render();
}

void Atlas::IMGUIRenderPass::draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
	VkRenderingAttachmentInfo colorAttachment = CreateAttachmentInfo(cDrawData.currentSwapchainImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	VkRenderingInfo renderInfo = CreateRenderingInfo(cDrawData.swapchainExtent, &colorAttachment, nullptr);

	vkCmdBeginRendering(cmd, &renderInfo);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
	vkCmdEndRendering(cmd);
}

void Atlas::IMGUIRenderPass::endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData)
{
	
}

