#pragma once
#include "WindowDecorations.h"

namespace Atlas {
	class Window {
	private:
		WindowDecorations mDecorations;

	protected:

	public:
		Window(WindowDecorations const& decorations) : mDecorations(decorations) {}
	};
}
