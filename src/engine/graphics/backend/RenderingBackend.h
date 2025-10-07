/**************************************************************************************************
 * @file RenderingBackend.h
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <optional>
#include <string>
#include <functional>
#include <cstdint>
#include <vector>
#include <initializer_list>


#include "../../core/Core.h"
#include "../../core/Version.h"

#include "../../debugging/Debugger.h"

#include "../window/Window.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif


namespace Atlas {

	//class AGameWindow;

	//enum class TextureFormat { RGBA8, Depth32 };

	//struct RenderTargetDesc { 
	//	int width;
	//	int height;
	//	TextureFormat format; 
	//};

	//class RenderTarget {
	//public:
	//	RenderTarget(RenderTargetDesc d) : desc(d) {}
	//	int getWidth() const { return desc.width; }
	//	int getHeight() const { return desc.height; }
	//	TextureFormat getFormat() const { return desc.format; }
	//private:
	//	RenderTargetDesc desc;
	//};

	//struct PipelineDesc { std::string vertPath, fragPath; };


	//class Pipeline {
	//public:
	//	Pipeline(PipelineDesc d) : desc(d) {}
	//private:
	//	PipelineDesc desc;
	//};

	//// RenderPass describes a single pass in the graph
	//struct RenderPass {
	//	std::string name;
	//	std::shared_ptr<Pipeline> pipeline;
	//	std::vector<std::shared_ptr<RenderTarget>> inputs;
	//	std::shared_ptr<RenderTarget> output;
	//	std::function<void()> onExecute;
	//};

	//class RenderingBackend;

	//// RenderGraph holds passes and executes them (sequentially by default)
	//class RenderGraph {
	//private:
	//	std::vector<RenderPass> passes;
	//public:
	//	auto createRenderTarget(RenderTargetDesc d) {
	//		return std::make_shared<RenderTarget>(d);
	//	}
	//	auto createPipeline(PipelineDesc d) {
	//		return std::make_shared<Pipeline>(d);
	//	}
	//	void addPass(const RenderPass& p) { passes.push_back(p); }

	//	const std::vector<RenderPass>& getPasses() const { return passes; }

	//	void execute() {
	//		for (auto& p : passes) {
	//			std::cout << "[RenderGraph] Executing: " << p.name << std::endl;
	//			if (p.onExecute) p.onExecute();
	//		}
	//	}
	//};



	class RenderingBackend {
	public:
		using APIVersion = Version;

	private:
		Version mAPIVersion;
		
		bool mbUseDebuggingTools = false;
		bool mbEnableErrorChecking = false; // dont worry about this if you are not using vulkan
	public:
		
		RenderingBackend() = default;

		

		virtual void setAPIVersion(uint32_t major, uint32_t minor, uint32_t patch);

		virtual void setAPIVersion(Version version);

		virtual Version getAPIVersion() const;

		bool areDebuggingToolsEnabled() const;

		bool isErrorCheckingEnabled() const;

		virtual void init(AGameWindow* windowHandle) = 0;

		virtual void update() = 0;
		
		virtual void shutdown() = 0;

		template<typename T_CAST_TO>
		T_CAST_TO* castAs() {
			return dynamic_cast<T_CAST_TO*>(this);
		}
	};


}