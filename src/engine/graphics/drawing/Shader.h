#pragma once
#include <vector>

#include "ShaderModule.h"

namespace Atlas {

	class ShaderBase : public NamedObject {
	private:
		std::vector<ShaderModule> mShaderModules;

	public:

		void pushShaderModule(ShaderModule shaderModule);

		void destroy();
	};

	// A grouping of shader modules
	class Shader : public ShaderBase {
	private:

		VkShaderModule mShaderModule = VK_NULL_HANDLE;
		//std::vector<VkShaderModule> mShaderModules;


	public:

		Shader() = default;

		//explicit Shader(std::filesystem::path compiledShaderPath, std::string name) : mCompiledShaderPath(compiledShaderPath), mName(name) {}

		//void destroyModule(VkDevice device) {
		//	vkDestroyShaderModule(device, mShaderModule, nullptr);
		//}

		//bool load(VkDevice device) {
		//	return LoadShaderModule(mCompiledShaderPath.string().c_str(), device, &mShaderModule);
		//}

		VkShaderModule getModule() const;

	};
}
