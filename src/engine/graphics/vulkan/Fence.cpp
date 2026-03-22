/**************************************************************************************************
 * @file Fence.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include "Fence.h"

void Atlas::Fence::ParseAndThrowCreationError(VkResult error) {
	switch (error)
	{
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw AException("Failed to create a fence because there was not enough device memory available.");
		break;

	case VK_ERROR_OUT_OF_HOST_MEMORY:
		throw AException("Failed to create a fence because there was not enough host memory available.");
		break;

		// The vkCreateFence() documentation's list of possible error codes states VK_ERROR_VALIDATION_FAILED as a possible error code,
		// and it is a valid code, but on some situations it is not, therefore, VK_ERROR_VALIDATION_FAILED_EXT is being used instead.
		// I do not know if this will cause issues down the line.
	case VK_ERROR_VALIDATION_FAILED_EXT:
		throw AException("Failed to create a fence because the validation layers detected an error.");
		break;

	case VK_ERROR_UNKNOWN:
		throw AException("Failed to create a fence for an unknown reason because VK_ERROR_UNKNOWN was returned by vkCreateFence().");
		break;
	default:
		throw AException("Failed to create a fence for an unknown reason. Please file a bug report.");
		break;
	}
}

void Atlas::Fence::ParseAndThrowWaitError(VkResult error) {
	switch (error)
	{
	case VK_ERROR_DEVICE_LOST:
		throw AException("Failed to wait for a fence because the device was lost. This can be caused by driver issues. Refer to the documentation of Fence::wait() or Vulkan's own documentation for more information.");
		break;

	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw AException("Failed to wait for a fence because there was not enough device memory available.");
		break;

	case VK_ERROR_OUT_OF_HOST_MEMORY:
		throw AException("Failed to wait for a fence because there was not enough host memory available.");
		break;

	case VK_ERROR_VALIDATION_FAILED_EXT:
		throw AException("Failed to wait for a fence because the validation layers detected an error.");
		break;

	case VK_ERROR_UNKNOWN:
		throw AException("Failed to wait for a fence for an unknown reason because VK_ERROR_UNKNOWN was returned by vkWaitForFences().");
		break;
	default:
		throw AException("Failed to wait for a fence for an unknown reason. Please file a bug report.");
		break;
	}
}

void Atlas::Fence::ParseAndThrowResetError(VkResult error) {
	switch (error)
	{
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw AException("Failed to reset a fence because there was not enough device memory available.");
		break;

	case VK_ERROR_UNKNOWN:
		throw AException("Failed to reset a fence for an unknown reason because VK_ERROR_UNKNOWN was returned by vkResetFences().");
		break;

	case VK_ERROR_VALIDATION_FAILED_EXT:
		throw AException("Failed to reset a fence because the validation layers detected an error.");
		break;
	default:
		break;
	}
}

Atlas::Fence::Fence(Device const& ownerDevice, const VkFenceCreateInfo* cCreateInfo, uint64_t timeoutInNS, const VkAllocationCallbacks* cAllocator) : mOwner(ownerDevice), mTimeoutInNS(timeoutInNS), cmAllocationCallbacks(cAllocator) {
	if (cCreateInfo == nullptr) {
		throw AException("Cannot create a fence with a null fence create info.");
	}

	if (!ownerDevice.isValid()) {
		throw AException("Cannot create a fence with an invalid device. Is it initialized?");
	}

	const VkResult cCreateResult = vkCreateFence(ownerDevice, cCreateInfo, cAllocator, &mFence);

	if (cCreateResult != VK_SUCCESS) {
		ParseAndThrowCreationError(cCreateResult);
	}
	else {
		InfoLog("Successfully created fence");
	}
}

void Atlas::Fence::destroy(VkDevice ownerDevice) {
	if (mFence == VK_NULL_HANDLE) {
		throw AException("Cannot destroy a fence that is already destroyed. Did you destroy it twice by accident?");
	}

	if (ownerDevice == VK_NULL_HANDLE) {
		throw AException("The device that created the fence is no longer valid. Cannot destroy the fence. This issue could also be caused by the device's address being lost.");
	}

	// This is to make sure that there is nothing left to wait on before destroying the fence
	this->wait(ownerDevice);

	vkDestroyFence(ownerDevice, mFence, cmAllocationCallbacks);
}

void Atlas::Fence::destroy() {
	destroy(mOwner);
}

void Atlas::Fence::wait(VkDevice ownerDevice, bool bWaitAll) {
	if (ownerDevice == VK_NULL_HANDLE) {
		throw AException("The device passed to use for waiting on the fence is invalid. Did you make sure to pass in the correct device?");
	}

	const VkResult cWaitResult = vkWaitForFences(ownerDevice, 1, &mFence, bWaitAll, mTimeoutInNS);

	const bool cbHasTimedOut = cWaitResult == VK_TIMEOUT;

	if (cWaitResult != VK_SUCCESS && !cbHasTimedOut) {
		ParseAndThrowWaitError(cWaitResult);
	}
	else if (cbHasTimedOut) {
		// TODO: Add the mentioned documentation
		WarnLog("Waiting for fence has timed out. Atlas does not immediately consider this an error, but it is worth checking and attempting to fix. Furthermore, please see the documentation for Fence::wait() in Atlas' docs for more information.");
	}
}

void Atlas::Fence::wait() {
	wait(mOwner);
}

void Atlas::Fence::reset(VkDevice ownerDevice) {
	if (ownerDevice == VK_NULL_HANDLE) {
		throw AException("Cannot reset a fence with an invalid device. Did you make sure to pass in the correct device?");
	}

	if (mFence == VK_NULL_HANDLE) {
		throw AException("Cannot reset a fence that is already destroyed. Did you destroy it twice by accident?");
	}

	// This is to make sure that there is nothing left to wait on before resetting the fence
	this->wait(ownerDevice, true);

	const VkResult cResetResult = vkResetFences(ownerDevice, 1, &mFence);

	if (cResetResult != VK_SUCCESS) {
		ParseAndThrowResetError(cResetResult);
	}
}

void Atlas::Fence::reset() {
	reset(mOwner);
}

void Atlas::Fence::setTimeout(const uint64_t cNewTimeoutInNS) noexcept {
	mTimeoutInNS = cNewTimeoutInNS;
}

Atlas::EFenceStatus Atlas::Fence::getStatus(VkDevice ownerDevice) const {
	const VkResult cStatus = vkGetFenceStatus(ownerDevice, mFence);

	// Translate the VkResult to an EFenceStatus
	switch (cStatus)
	{
	case VK_SUCCESS:
		return EFenceStatus::Ready;
	case VK_NOT_READY:
		return EFenceStatus::NotReady;

	case VK_ERROR_DEVICE_LOST:
		return EFenceStatus::DeviceLost;
	default:
		return EFenceStatus::Unknown;
	}
}

bool Atlas::Fence::hasDeviceBeenLost(VkDevice ownerDevice) const {
	return getStatus(ownerDevice) == EFenceStatus::DeviceLost;
}

uint64_t Atlas::Fence::getTimeout() const noexcept {
	return mTimeoutInNS;
}

VkFence Atlas::Fence::getFence() const noexcept {
	return mFence;
}
