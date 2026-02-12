#pragma once

#include "../../core/Core.h"

#include "../Renderer.h"

#include "VKDevice.h"

namespace Atlas {
#ifdef ATLAS_USE_VULKAN

	class VulkanRenderer : public ARenderer {
	private:
		static bool WindowShouldClose(GameWindow* gameWindow)
		{
			return gameWindow->shouldClose();
		}

		uint64_t mCurrentFrameNumber = 0;
	public:
		//using ARenderer::ARenderer;

		VulkanRenderer(GameWindow* gameWindow, VulkanRenderingBackend* backend);

		explicit VulkanRenderer(GameWindow* gameWindow);

		void init() override;

		void update() override;

		void cleanup() override;

		bool shouldExit() override;
	};

#endif
}
