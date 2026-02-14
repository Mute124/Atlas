#pragma once
#include <string>

#include "../RenderCommon.h"
#include "DrawData.h"
#include "EffectManager.h"

namespace Atlas {
	class Renderable {
	private:
		std::string mName;
	protected:
		friend class VulkanRenderingBackend;

		void setName(std::string const& name) { mName = name; }
	public:

		ATLAS_EXPLICIT Renderable(std::string const& name) : mName(name) {}
		Renderable() : Renderable("Unnamed Renderable") {}

		static inline void SetCurrentDrawData(CurrentDrawData const& currentFrame) {
			//std::scoped_lock lock(sCurrentDrawDataMutex);
			//sCurrentDrawData = currentFrame;
		}

		virtual bool beginDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData, EffectManager& computeEffects) {
			return 0;
		}

		virtual bool draw(VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			return 0;
		}

		virtual bool endDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData) {
			return 0;
		}

		std::string_view getName() const { return mName; }
	};
}
