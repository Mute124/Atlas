#include "AllocatedBuffer.h"

#include "../../debugging/Logging.h"

inline VkBufferCreateInfo Atlas::AllocatedBuffer::CreateBufferCreateInfo(size_t allocSize, VkBufferUsageFlags usage) {
	if (allocSize <= 0) {
		throw InvalidAllocationException("Cannot create buffer with an allocation size less than or equal to 0!");
	}
	
	VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.pNext = nullptr;
	bufferInfo.size = allocSize;
	bufferInfo.usage = usage;
	return bufferInfo;
}

inline VmaAllocationCreateInfo Atlas::AllocatedBuffer::CreateAllocationCreateInfo(VmaMemoryUsage memoryUsage) {
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = memoryUsage;
	vmaallocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	return vmaallocInfo;
}

Atlas::AllocatedBuffer::AllocatedBuffer(VmaAllocator vmaAllocator, VkBufferCreateInfo const& bufferInfo, VmaAllocationCreateInfo const& vmaallocInfo) : mAllocator(vmaAllocator) {
	if (vmaAllocator == nullptr) {
		throw InvalidAllocatorException("Cannot create a buffer with a null allocator. Did you make sure that the VMA allocator is initialized and that you pass in a valid allocator?");
	}

	vmaCreateBuffer(vmaAllocator, &bufferInfo, &vmaallocInfo, &mBuffer, &mAllocation, &mInfo);
}

Atlas::AllocatedBuffer::AllocatedBuffer(VmaAllocator vmaAllocator, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
	: AllocatedBuffer(vmaAllocator, CreateBufferCreateInfo(allocSize, usage), CreateAllocationCreateInfo(memoryUsage)) {}

void Atlas::AllocatedBuffer::destroy(VmaAllocator allocator) const {
	if (mBuffer == nullptr) {
		throw InvalidBufferException("Attempting to destroy buffer, but the buffer is null. Did you forget to create it or did you destroy it twice?");
	}

	if (allocator == nullptr) {
		throw InvalidAllocatorException("Cannot destroy a buffer with a null allocator. Make sure that you pass in a valid allocator pointer when creating the buffer.");
	}

	vmaDestroyBuffer(allocator, mBuffer, mAllocation);
}

void Atlas::AllocatedBuffer::destroy() const {
	// Any errors will be caught in the following function.
	destroy(mAllocator);
}

VkBuffer Atlas::AllocatedBuffer::getBuffer() const {
	return mBuffer;
}

VmaAllocation Atlas::AllocatedBuffer::getAllocation() const {
	return mAllocation;
}

VmaAllocationInfo Atlas::AllocatedBuffer::getAllocationInfo() const {
	return mInfo;
}