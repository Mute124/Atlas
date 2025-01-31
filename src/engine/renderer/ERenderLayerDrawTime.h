#pragma once
#include <stdint.h>

namespace Atlas {
	enum class ERenderLayerDrawTime : uint8_t
	{
		UNKNOWN = 0,
		TEXTURE = 1,
		RENDER = 2
	};
}