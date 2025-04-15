#pragma once
#include <string>

#include "../../core/Core.h"

namespace Atlas {
	class IRenderingBackend {
	protected:
		std::string mAPIName;
	public:

		virtual void init() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;
	};
}