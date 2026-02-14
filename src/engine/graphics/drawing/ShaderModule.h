#pragma once
#include <filesystem>

#include "../../io/IOManager.h"

#include "../vulkan/PhysicalDevice.h"
#include "EShaderModuleType.h"

namespace Atlas {
	class ShaderModule {
	private:
		friend class ShaderBase;

		Device mDevice;

		VkShaderModule mShaderModule{ VK_NULL_HANDLE };
		EShaderModuleType mModuleType{ EShaderModuleType::Vertex };

		bool mbLoaded{ false };
	public:

		ShaderModule(Device const& device, std::filesystem::path const& path, FileManager& ioManager, EShaderModuleType moduleType);

		ShaderModule() = default;

		bool createShaderModule(Device const& device, VkShaderModuleCreateInfo* createInfo, VkShaderModule* module);

		bool load(std::filesystem::path const& path, FileManager& ioManager);

		bool destroy();

		bool isLoaded() const;

		VkShaderModule getShaderModule() const;

		EShaderModuleType getShaderModuleType() const;

		explicit(false) operator VkShaderModule() const;
	};
}
