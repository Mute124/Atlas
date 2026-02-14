#pragma once
#include <memory>

#include "../../core/Common.h"
#include "Pipeline.h"
#include "DrawData.h"

namespace Atlas {

	using RenderPassIndex = size_t;

	class RenderPass : public NamedObject, public Validatable {
		friend class RenderPassesManager;
	private:
		std::shared_ptr<Pipeline> mPipeline{ nullptr };

		RenderPassIndex mIndex;

	protected:
		void setIndex(RenderPassIndex index);

	public:
		RenderPass() = default;

		virtual void beginRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		virtual void draw(const VkCommandBuffer cmd, CurrentDrawData& cDrawData) = 0;

		virtual void endRenderPass(const VkCommandBuffer cmd, CurrentDrawData& cDrawData);

		RenderPassIndex getID() const;
	};
}
