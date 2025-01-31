#pragma once
#include "WindowDecorations.h"
#include "IWindow.h"
#include <optional>
namespace Atlas {
	class Window : public IWindow {
	private:
		WindowDecorations mDecorations;

	public:
		
		~Window();

		WindowDecorations const& GetDecorations() const { return mDecorations; }



		// Inherited via IWindow
		void init(WindowDecorations* windowDecorations) override;

		void update() override;

		bool shouldClose() override;

		void close() override;

	};
}
