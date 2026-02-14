#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <vector>

#include "../Frame.h"
#include "../vulkan/AllocatedImage.h"
#include "EffectManager.h"

namespace Atlas {
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
}
