/**************************************************************************************************
 * @file VulkanSwapchain.h
 * 
 * @brief .
 * 
 * @date April 2026
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
 * ***************************************************************************************************/
#pragma once
#include <optional>
#include <vector>

#include <glm/fwd.hpp>
#include <vulkan/vulkan.h>

#include  "../../core/Common.h"
#include "../../core/Math.h"

#include "EFormat.h"

#include "AllocatedImage.h"

namespace Atlas {
	


	class Swapchain : public InitializableAndValidatable {
	private:
		VkSwapchainKHR mSwapchain{ VK_NULL_HANDLE };
		
		std::vector<VkImage> mSwapchainImages;
		std::vector<VkImageView> mSwapchainImageViews;

		EPixelFormat mPixelFormat;

		// Z Value is depth
		Vector3 mSize;


	public:


		void destroy(VkDevice device, std::optional<const VkAllocationCallbacks*> allocator);
	};

}

