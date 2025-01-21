#pragma once
#include <iostream>
#include <memory>

#include "../IDrawable.h"

namespace Atlas {
	class DrawableHook {
	private:
		std::shared_ptr<IDrawable> drawable;
	public:

	};
}