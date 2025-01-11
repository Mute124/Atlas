#pragma once
#include <raylib.h>
namespace Atlas {
	class IChunk abstract {
	public:
		virtual void generate() = 0;
	};
}