#include "Shader.h"

void Atlas::ShaderBase::pushShaderModule(ShaderModule shaderModule) {
	mShaderModules.push_back(shaderModule);
}

void Atlas::ShaderBase::destroy() {
	for (auto& shaderModule : mShaderModules) {
		shaderModule.destroy();
	}
}

VkShaderModule Atlas::Shader::getModule() const { return mShaderModule; }
