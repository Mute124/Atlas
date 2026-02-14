#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <vk_mem_alloc.h>

#include "../../debugging/AException.h"

namespace Atlas {

	class InvalidBufferException : public AException {
	public:
		using AException::AException;
	};

	class InvalidAllocatorException : public AException {
	public:
		using AException::AException;
	};

	class InvalidAllocationException : public AException {
	public:
		using AException::AException;
	};

	class AllocatedBuffer final {
	private:

		VkBuffer mBuffer{ VK_NULL_HANDLE };
		VmaAllocator mAllocator{ nullptr };
		VmaAllocation mAllocation{ nullptr };
		VmaAllocationInfo mInfo{};

		AllocatedBuffer(VmaAllocator vmaAllocator, VkBufferCreateInfo const& bufferInfo, VmaAllocationCreateInfo const& vmaallocInfo);

		static VkBufferCreateInfo CreateBufferCreateInfo(size_t allocSize, VkBufferUsageFlags usage);

		static VmaAllocationCreateInfo CreateAllocationCreateInfo(VmaMemoryUsage memoryUsage);
	public:

		AllocatedBuffer(VmaAllocator vmaAllocator, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		AllocatedBuffer() = default;

		// I dont know why the hell having a deconstructor causes Vulkan to freak out when uploading a mesh...

		void destroy(VmaAllocator allocator) const;

		void destroy() const;

		VkBuffer getBuffer() const;

		VmaAllocation getAllocation() const;

		VmaAllocationInfo getAllocationInfo() const;
	};
}
