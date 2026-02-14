#include "ShaderModule.h"

Atlas::ShaderModule::ShaderModule(Device const& device, std::filesystem::path const& path, FileManager& ioManager, EShaderModuleType moduleType) : mDevice(device), mModuleType(moduleType) {
	load(path, ioManager);
}

bool Atlas::ShaderModule::createShaderModule(Device const& device, VkShaderModuleCreateInfo* createInfo, VkShaderModule* module) {
	ATLAS_ASSERT(createInfo != nullptr, "createInfo must not be nullptr!");
	ATLAS_ASSERT(module != nullptr, "The VkShaderModule passed must not be nullptr!");

	return vkCreateShaderModule(device, createInfo, nullptr, module) == VK_SUCCESS;
}

bool Atlas::ShaderModule::load(std::filesystem::path const& path, FileManager& ioManager) {
	FileHandle handle = ioManager.openFile(path);

	std::shared_ptr<FileData> fileData = handle.get();

	// create a new shader module, using the buffer we loaded
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	// codeSize has to be in bytes, so multply the ints in the buffer by size of
	// int to know the real size of the buffer
	createInfo.codeSize = fileData->size() * sizeof(uint32_t);

	// Since SPIRV needs to be uint32_t, we need to convert the bytes to uint32_t
	std::vector<uint32_t> code;

	for (size_t i = 0; i < fileData->bytes.size(); i++) {
		code.push_back(fileData->bytes[i].to<uint32_t>());
	}

	createInfo.pCode = code.data();

	if (!createShaderModule(mDevice, &createInfo, &mShaderModule)) {
		return false;
	}

	mbLoaded = true;

	return true;
}

bool Atlas::ShaderModule::destroy() {
	bool success = false;
	if (isLoaded()) {
		vkDestroyShaderModule(mDevice, mShaderModule, nullptr);

		mbLoaded = false;
		success = true;
	}

	return success;
}

bool Atlas::ShaderModule::isLoaded() const {
	return mbLoaded;
}

VkShaderModule Atlas::ShaderModule::getShaderModule() const {
	return mShaderModule;
}

Atlas::EShaderModuleType Atlas::ShaderModule::getShaderModuleType() const {
	return mModuleType;
}

Atlas::ShaderModule::operator VkShaderModule() const {
	return mShaderModule;
}
