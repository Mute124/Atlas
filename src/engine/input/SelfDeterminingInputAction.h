#pragma once
#include "InputAction.h"
#include <functional>

namespace Atlas {
	class SelfDeterminingInputAction : public InputAction {
	public:

		std::function<bool(const SelfDeterminingInputAction*)> mChecker;
		std::function<void(const SelfDeterminingInputAction*)> mCallback;

		SelfDeterminingInputAction(
			std::function<bool(const SelfDeterminingInputAction*)> const& checker,
			std::function<void(const SelfDeterminingInputAction*)> const& callback
		);

		SelfDeterminingInputAction() = default;

		bool check() const;

		void callback() const;
	};
}