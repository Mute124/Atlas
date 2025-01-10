#pragma once

#include "../Common.h"

namespace Atlas {
	/// <summary>
	/// Window decorations are a set of parameters that are used to configure the window. This includes title, width, height, window config, and target FPS.
	/// </summary>
	struct WindowDecorations {
		const char* title = "Untitled Atlas Project";
		uint32_t width = 1280; /// <summary> The width of the window. </summary>
		uint32_t height = 720; /// <summary> The height of the window. </summary>
		unsigned int windowConfig = -1; ///<summary> For available config flags, see <see cref="ConfigFlags"/> in the raylib documentation.</summary>
		unsigned int targetFPS = 60; /// <summary> The target FPS for the window. </summary>
		const char* icon = "engine/techstorm.png";
	};
}
