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

		void setName(std::string const& name);
	public:

		ATLAS_EXPLICIT Renderable(std::string const& name);
		Renderable();

		static inline void SetCurrentDrawData(CurrentDrawData const& currentFrame);

		virtual bool beginDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData, EffectManager& computeEffects);

		virtual bool draw(VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		virtual bool endDrawingStage(VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		std::string_view getName() const;
	};
}
