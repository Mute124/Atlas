#pragma once
#include "WindowDecorations.h"
#include "IWindow.h"
namespace Atlas {
	class Window {
	private:
		const WindowDecorations& mDecorations;

	protected:

	public:
		Window(WindowDecorations const& decorations) : mDecorations(decorations) {}

		WindowDecorations const& GetDecorations() const { return mDecorations; }


	};
}
