#pragma once

#include "RenderingBackend.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif

#ifdef ATLAS_USE_VULKAN

namespace Atlas {
	class VulkanRenderingBackend : public RenderingBackend {
	private:
		class ValidationLayers {
		private:

			std::vector<const char*> mValidationLayers;

		public:

			ValidationLayers(std::initializer_list<const char*> layers) : mValidationLayers(layers) {}

			/**
			 * @brief Gets a copy of the validation layers vector.
			 *
			 * @return
			 *
			 * @since v
			 */
			std::vector<const char*> getLayers() const {
				return mValidationLayers;
			}

		};

		ValidationLayers* mValidationLayers = nullptr;

		// Vulkan stuff
		VkInstance mVulkanInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT mVulkanDebugMessenger = VK_NULL_HANDLE;
		VkPhysicalDevice mVulkanGPUDevice = VK_NULL_HANDLE;
		VkDevice mVulkanDevice = VK_NULL_HANDLE;
		VkSurfaceKHR mVulkanSurface = VK_NULL_HANDLE;

		VkSwapchainKHR mVulkanSwapchain;
		VkFormat mVulkanSwapchainImageFormat;

		std::vector<VkImage> mVulkanSwapchainImages;
		std::vector<VkImageView> mVulkanSwapchainImageViews;
		VkExtent2D mVulkanSwapchainExtent;

		std::string mApplicationName;

		//std::vector<const char*> mValidationLayers;

		uint16_t initInstance(const APIVersion& cAPIVersionRef, bool cbEnableValidationLayers, std::string appName);
		
		bool mIsInitialized = false;

		void createSwapchain(uint32_t width, uint32_t height);
		void destroySwapchain();

	public:

		VulkanRenderingBackend(const VulkanRenderingBackend&) = delete;

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

		void setApplicationName(std::string_view applicationName);

		std::string getApplicationName();

		void init(AGameWindow* windowHandle) override;

		void update() override;

		void shutdown() override;

		bool checkValidationLayerSupport();
	};
}
#endif