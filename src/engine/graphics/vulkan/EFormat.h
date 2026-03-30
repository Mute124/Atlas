/**************************************************************************************************
 * @file EFormat.h
 * 
 * @brief .
 * 
 * @date March 2026
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

#include <cstdint>

// 1,000,609,013
namespace Atlas {
	/**
	 * @brief An enum class that contains all possible formats. The values are based off of Vulkan's VkFormat
	 * enum, and should be the same. The reason for this being in existence is to make the code more readable
	 * and make it so that the Vulkan's API is not exposed to the user.
	 * 
	 * Currently, only Vulkan 1.2 and above is supported, meaning any formats that are not in Vulkan 1.2 or
	 * above will not be included in this enum.
	 * 
	 * @note The uint32_t may have to be changed, I do not know. This was just an educated guess I made based
	 * off of Vulkan's VkFormat enum's documentation.
	 * 
	 * @todo Work on this class:
	 * 
	 * @since v
	 */
	enum class EFormat : uint32_t {
		Unknown = 0,
	};
}