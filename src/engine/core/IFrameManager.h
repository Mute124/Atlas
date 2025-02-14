#pragma once
#include "../utils/Singleton.h"
#define ATLAS_THREAD_YIELD std::this_thread::yield()

namespace Atlas {

	template<typename T>
	class IFrameManager : public Singleton<T> {
	public:
		virtual ~IFrameManager() = default;
	};
}