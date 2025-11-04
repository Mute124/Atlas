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
#include <memory>
#include <type_traits>
#include <mutex>
#include <atomic>

#include "../../core/Core.h"
#include "../../core/Common.h"
#include "../../core/Version.h"

#include "../../core/threading/ThreadSafeVariable.h"

#include "../../debugging/Debugger.h"
#include "../../debugging/Logging.h"
#include "../window/Window.h"


#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif
#include <shared_mutex>





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



	struct RendererSettings {
		// Empty for now

		
		bool bUseDebuggingTools = false;
		bool bEnableErrorChecking = false;
	};

	class RenderingBackend {
	public:
		using APIVersion = Version;

		struct State {
			std::shared_ptr<AGameWindow> gameWindow;
			RendererSettings settings;
			
			bool bIsReady{ false };
		};

	protected:


		std::shared_ptr<State> mState;
		std::shared_mutex mStateMutex;

		Version mAPIVersion;
			
		bool mbIsInitialized = false;
		bool mbUseDebuggingTools = false;
		bool mbEnableErrorChecking = false; // dont worry about this if you are not using vulkan
	public:

		RenderingBackend() = default;

		virtual ~RenderingBackend() {
			shutdown();
		}

		//virtual void init(AGameWindow* windowHandle)
		//{
		//	ATLAS_ASSERT(windowHandle != nullptr, "Window handle cannot be null!");

		//	mbIsInitialized = true;
		//}
		virtual void init(AGameWindow* gameWindow) = 0;
		

		virtual void beginDrawing() {}

		virtual void draw() {}

		virtual void endDrawing() {}

		virtual void shutdown() {
			mbIsInitialized = false;
		}

		virtual void setAPIVersion(uint32_t major, uint32_t minor, uint32_t patch);

		virtual void setAPIVersion(Version version);

		virtual Version getAPIVersion() const;

		//inline std::shared_ptr<AGameWindow> getGameWindow() const { return mGameWindowPtr; }

		bool areDebuggingToolsEnabled() const;

		bool isErrorCheckingEnabled() const;

		template<typename T_CAST_TO>
		T_CAST_TO* castAs() {
			return dynamic_cast<T_CAST_TO*>(this);
		}
	};
	
	template<typename T_TARGET>
	concept ValidRenderer = std::is_base_of_v<RenderingBackend, T_TARGET>;

	template<typename T_CHILD>
	class AGlobalRenderingBackend : public RenderingBackend {
	protected:
		static inline std::shared_ptr<T_CHILD> sActiveInstanceTSSV{};
		static inline std::shared_mutex sActiveInstanceMutex{};

		static inline void SetGlobalInstance(const std::shared_ptr<T_CHILD>& instance) {
			//std::scoped_lock lock(sActiveInstanceTSSV.lock());
			sActiveInstanceTSSV.get().set(instance);
		}

	public:

		using RenderingBackend::RenderingBackend;

		static inline std::shared_ptr<T_CHILD> GetGlobalInstance() {
			return sActiveInstanceTSSV;
		}

		static inline bool HasGlobalInstance() {
			return sActiveInstanceTSSV.get() != nullptr;
		}

		static inline void ClearGlobalInstance() {
			//std::scoped_lock lock(sActiveInstanceTSSV.lock());
			//sActiveInstanceTSSV = nullptr;
			//sActiveInstanceTSSV.reset();

			sActiveInstanceTSSV = nullptr;
		}

		virtual void init(AGameWindow* windowHandle) override {			
			if (HasGlobalInstance()) {
				ErrorLog("RenderingBackend has already been initialized");
				return;
			}
			else {
				setThisAsGlobalInstance();
			}

			RenderingBackend::init(windowHandle);
		}

		void setThisAsGlobalInstance() {
			SetGlobalInstance(std::make_shared<T_CHILD>(this));
		}

		virtual void shutdown() override {
			RenderingBackend::shutdown();
			ClearGlobalInstance();
		}
	};
}