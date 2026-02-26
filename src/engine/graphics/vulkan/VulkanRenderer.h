#pragma once

#include <cstdint>
#include <memory>

#include "../../core/Core.h"
#include "../../core/Common.h"

#include "../window/Window.h"

#include "VKDevice.h"

namespace Atlas {
	class VulkanRenderer {
	public:
		struct Configuration {
			ApplicationInfo appInfo;
		};
	private:
		Counter<uint64_t> mFrameCounter{};
	public:

		void init(std::unique_ptr<GameWindow> window) {
			if (window == nullptr || !window->isInit()) {
				throw std::runtime_error("Window not initialized");
			}


		}
	};
}
