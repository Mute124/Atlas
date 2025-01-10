#pragma once
#define TS_RENDERER_RAYLIB
#ifdef TS_RENDERER_RAYLIB

#endif

namespace Atlas {
	/// <summary>
	///
	/// </summary>
	/// @deprecated Do not use this class as it will be removed.
	class Window {
	public:

		/// <summary>
		/// Finalizes an instance of the <see cref="Window"/> class.
		/// </summary>
		/// @deprecated See class deprecation warning
		~Window();

		/// <summary>
		/// Initializes the specified width, height, title, and window configuration flags.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="title">The title.</param>
		/// <param name="windowConfig">The window configuration.</param>
		/// <returns></returns>
		/// @deprecated See class deprecation warning
		int initialize(int width = 600, int height = 800, const char* title = "Untitled Atlas Project", unsigned int windowConfig = 0);

		/// <summary>
		/// Should the window close.
		/// </summary>
		/// <returns>The value of WindowShouldClose()</returns>
		/// @deprecated See class deprecation warning
		bool shouldClose();

		/// <summary>
		/// Determines whether this instance is ready.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is ready; otherwise, <c>false</c>.
		/// </returns>
		/// @deprecated See class deprecation warning
		bool isReady();

	private:

		int mWidth = 600;
		int mHeight = 800;
		const char* mTitle = "Untitled Atlas Project";
		unsigned int mWindowConfig;
	};
}
