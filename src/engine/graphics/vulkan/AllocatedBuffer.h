#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <vk_mem_alloc.h>

#include "../../debugging/Logging.h"
namespace Atlas {

	class AllocatedBuffer final {
	private:

		VkBuffer mBuffer{ VK_NULL_HANDLE };
		VmaAllocator mAllocator{ nullptr };
		VmaAllocation mAllocation{ nullptr };
		VmaAllocationInfo mInfo{};

		AllocatedBuffer(VmaAllocator vmaAllocator, VkBufferCreateInfo const& bufferInfo, VmaAllocationCreateInfo const& vmaallocInfo) : mAllocator(vmaAllocator) {
			vmaCreateBuffer(vmaAllocator, &bufferInfo, &vmaallocInfo, &mBuffer, &mAllocation, &mInfo);
		}

		static inline VkBufferCreateInfo CreateBufferCreateInfo(size_t allocSize, VkBufferUsageFlags usage) {
			VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
			bufferInfo.pNext = nullptr;
			bufferInfo.size = allocSize;
			bufferInfo.usage = usage;
			return bufferInfo;
		}

		static inline VmaAllocationCreateInfo CreateAllocationCreateInfo(VmaMemoryUsage memoryUsage) {
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = memoryUsage;
			vmaallocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
			return vmaallocInfo;
		}
	public:

		AllocatedBuffer(VmaAllocator vmaAllocator, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) 
			: AllocatedBuffer(vmaAllocator, CreateBufferCreateInfo(allocSize, usage), CreateAllocationCreateInfo(memoryUsage)) {}

		AllocatedBuffer() = default;

		// I dont know why the hell having a deconstructor causes Vulkan to freak out when uploading a mesh...

		void destroy(VmaAllocator allocator) const {
			if (allocator == nullptr) {
				ErrorLog("Allocator is null");
			}

			vmaDestroyBuffer(allocator, mBuffer, mAllocation);
		}

		void destroy() const {
			destroy(mAllocator);
		}

		VkBuffer getBuffer() const {
			return mBuffer;
		}

		VmaAllocation getAllocation() const {
			return mAllocation;
		}

		VmaAllocationInfo getAllocationInfo() const {
			return mInfo;
		}
	};
}
