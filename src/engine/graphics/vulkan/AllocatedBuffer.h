#pragma once
#include <cstdint>

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

	class ForbiddenBufferAccessException : public AException {
	public:
		using AException::AException;
	};

	class AAllocatedBuffer {
	private:
		VkBuffer mBuffer{ VK_NULL_HANDLE };
		VmaAllocation mAllocation{ nullptr };
		VmaAllocationInfo mInfo{};

	protected:

		void create(VmaAllocator vmaAllocator, VkBufferCreateInfo const& bufferInfo, VmaAllocationCreateInfo const& vmaallocInfo);

		void setBuffer(VkBuffer buffer);
	public:
		
		AAllocatedBuffer(VmaAllocator vmaAllocator, VkBufferCreateInfo const& bufferInfo, VmaAllocationCreateInfo const& vmaallocInfo);

		virtual void destroy(VmaAllocator allocator);

		bool isMemoryMapped() const noexcept;

		VkBuffer getBuffer() const;

		void* getMappedMemory() const;

		uint64_t getAllocationSize() const;
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

		VkBuffer& getBuffer();

		VmaAllocation getAllocation() const;

		VmaAllocationInfo getAllocationInfo() const;
	};
}
