#pragma once
#include "WindowDecorations.h"

namespace Atlas {
	class IWindow {
	public:
		virtual void init(WindowDecorations* windowDecorations) = 0;

		virtual void update() = 0;

		virtual bool shouldClose() = 0;

		virtual void close() = 0;

	};
}